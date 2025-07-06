#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024], guess[64], name[64];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    // Send name to server
    sendto(sockfd, name, strlen(name), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Welcome %s! Start guessing the 5-letter word.\n", name);

    while (1) {
        printf("Enter your guess: ");
        fgets(guess, sizeof(guess), stdin);
        guess[strcspn(guess, "\n")] = '\0';

        sendto(sockfd, guess, strlen(guess), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        socklen_t len = sizeof(server_addr);
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&server_addr, &len);
        buffer[n] = '\0';

        printf("\nServer:\n%s\n", buffer);

        if (strstr(buffer, "Congratulations!") ||
            strstr(buffer, "has won the game") ||
            strstr(buffer, "The correct word was")||
            strstr(buffer, "Too late! Game is over")) {
            break;
        }
    }

    close(sockfd);
    return 0;
}
