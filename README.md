## 🧠 Multi-Client UDP Word Guessing Game


A fast-paced, real-time word guessing game using UDP Sockets in C. Multiple clients connect to a central server over LAN. The server sends scrambled English words, and clients race to guess the original word. The fastest correct guess ends the game.

## Features

 - Multi-client UDP server (lightweight and efficient)

 - Name registration before gameplay

 - Scrambled 5-letter English word changes every game

 - First correct guess wins the game — broadcasted instantly to all clients

 - Multiple attempts allowed until someone wins the game

## Project Structure
```
new/
├── server.c           # Main UDP server that manages game logic
├── client.c           # Client program to guess words and receive updates
├── README.md          # This file - project instructions and overview
```
## Requirements
- Operating System: Linux or Unix-based (tested on Ubuntu)
- Compiler: gcc

##  How to Play

1. Start the Server
  ```
  ./server
  ```

- The server will begin and prepare the first scrambled word.

2. Start Clients
  ```
 ./client
  ```

- Enter your name to register.

- Wait for the scrambled word to appear.

- Type your guess and hit enter.

- If you’re correct, all players are notified that you have already guessed the correct word.

## Gameplay Flow

- Game has only 1 round.

- A random 5-letter English word is selected and scrambled.

- All clients see the same scrambled word.

- The first client to guess correctly wins the game.

- Others are notified if they guess too late or incorrect.



## Graceful Shutdown
- Use Ctrl+C to close the server or client safely.

- All sockets close properly, no resource leaks.

## Notes
- UDP is used for simplicity and performance. Message delivery is not guaranteed, but works well in local networks.

- Ideal for learning socket programming and game synchronization .

## Conclusion
This UDP Word Guessing Game offers a simple yet engaging way to learn network programming using sockets in C. With real-time updates, multiple client handling, and word-based logic, it’s a great project for practicing communication protocols and interactive gameplay logic.
