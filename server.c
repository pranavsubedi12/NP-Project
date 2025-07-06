#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_CLIENTS 10
#define MAX_ATTEMPTS 5
#define PORT 5555

// List of meaningful 5-letter words
const char* word_list[] = {
    "apple", "grape", "house", "plant", "light",
    "chair", "table", "river", "sound", "world",
    "beach", "clock", "dance", "earth", "field"
};
const int word_count = sizeof(word_list) / sizeof(word_list[0]);

typedef struct {
    struct sockaddr_in addr;
    int attempts;
    int active;
    char name[64];
    int name_received;
} Client;

Client clients[MAX_CLIENTS];
int game_over = 0;
char secret_word[6];  // 5 letters + null terminator

int same_client(struct sockaddr_in *a, struct sockaddr_in *b) {
    return a->sin_addr.s_addr == b->sin_addr.s_addr &&
           a->sin_port == b->sin_port;
}

int find_client(struct sockaddr_in *addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && same_client(&clients[i].addr, addr)) {
            return i;
        }
    }
    return -1;
}

int add_client(struct sockaddr_in *addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].addr = *addr;
            clients[i].attempts = 0;
            clients[i].active = 1;
            clients[i].name_received = 0;
            clients[i].name[0] = '\0';
            return i;
        }
    }
    return -1;
}

void send_to_all(int sockfd, char *msg, struct sockaddr_in *exclude) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active &&
            (!exclude || !same_client(&clients[i].addr, exclude))) {
            sendto(sockfd, msg, strlen(msg), 0,
                   (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
}

void pick_random_word() {
    srand(time(NULL));
    int idx = rand() % word_count;
    strncpy(secret_word, word_list[idx], 6);
}

void process_message(int sockfd, char *msg, struct sockaddr_in *client_addr) {
    char feedback[512] = "";
    int client_index = find_client(client_addr);
    if (client_index == -1) {
        client_index = add_client(client_addr);
    }

    Client *client = &clients[client_index];

    // If name not received yet, first message is the name
    if (!client->name_received) {
        strncpy(client->name, msg, sizeof(client->name) - 1);
        client->name_received = 1;
        printf("Client [%s:%d] is named '%s'\n",
               inet_ntoa(client_addr->sin_addr),
               ntohs(client_addr->sin_port),
               client->name);
        return;
    }

    // Show guess on server
    printf("Guess from %s [%s:%d]: %s\n",
           client->name,
           inet_ntoa(client_addr->sin_addr),
           ntohs(client_addr->sin_port),
           msg);

    if (game_over) {
        sprintf(feedback, "Game is over. The correct word was '%s'.", secret_word);
        sendto(sockfd, feedback, strlen(feedback), 0,
               (struct sockaddr *)client_addr, sizeof(*client_addr));
        return;
    }

    if (client->attempts >= MAX_ATTEMPTS) {
        sprintf(feedback, "Game is over. The correct word was '%s'.", secret_word);
        sendto(sockfd, feedback, strlen(feedback), 0,
               (struct sockaddr *)client_addr, sizeof(*client_addr));
        return;
    }

    if (strlen(msg) != 5) {
        sprintf(feedback, "Invalid guess. Must be 5 letters.");
        sendto(sockfd, feedback, strlen(feedback), 0,
               (struct sockaddr *)client_addr, sizeof(*client_addr));
        return;
    }

    for (int i = 0; i < 5; i++) {
        if (msg[i] == secret_word[i]) {
            char line[64];
            sprintf(line, "%c - Correct Place\n", msg[i]);
            strcat(feedback, line);
        } else if (strchr(secret_word, msg[i])) {
            char line[64];
            sprintf(line, "%c - Wrong Place\n", msg[i]);
            strcat(feedback, line);
        } else {
            char line[64];
            sprintf(line, "%c - Not in Word\n", msg[i]);
            strcat(feedback, line);
        }
    }

    client->attempts++;

    if (strncmp(msg, secret_word, 5) == 0) {
        // Notify winner
        sprintf(feedback, "Congratulations! You have guessed the word correctly");
        sendto(sockfd, feedback, strlen(feedback), 0,
               (struct sockaddr *)client_addr, sizeof(*client_addr));

        // Notify others
        char win_msg[256];
        sprintf(feedback, "Too late! Game is over. The correct word '%s' was already guessed by %s.", secret_word, client->name);
        send_to_all(sockfd, feedback, client_addr);
        

        game_over = 1;
    } 
    else {
        char attempts_left[64];
        sprintf(attempts_left, "Attempts left: %d\n", MAX_ATTEMPTS - client->attempts);
        strcat(feedback, attempts_left);
        sendto(sockfd, feedback, strlen(feedback), 0,
               (struct sockaddr *)client_addr, sizeof(*client_addr));
    }
}


int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(1);
    }

    pick_random_word();
    printf("UDP Server started on port %d\n", PORT);
    printf("Secret word chosen: %s\n", secret_word);  

    while (1) {
        socklen_t len = sizeof(client_addr);
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0';

        process_message(sockfd, buffer, &client_addr);
    }

    close(sockfd);
    return 0;
}
