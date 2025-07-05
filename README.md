## 🧠 Multi-Client UDP Word Guessing Game 

A fast-paced, real-time word guessing game using UDP Sockets in C. Multiple clients connect to a central server over LAN. The server sends scrambled English words, and clients race to guess the original word. The fastest correct guess ends the round and earns a point.

## Features

 - Multi-client UDP server (lightweight and efficient)

 - Name registration before gameplay

 - Scrambled 5-letter English word changes every round

 - First correct guess wins the round — broadcasted instantly to all clients

 - 5 Rounds per game with winner announced at the end

 - Late guesses receive “Too late” message after round ends

 - Multiple attempts allowed per round until someone wins

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

- If you’re correct, all players are notified and the next round starts.

## Gameplay Flow

- Game has 5 rounds.

- Each round:

  - A random 5-letter English word is selected and scrambled.

  - All clients see the same scrambled word.

  - The first client to guess correctly wins that round.

  - Others are notified if they guess too late.

- After 5 rounds, the overall winner is announced based on number of wins.

## Graceful Shutdown
- Use Ctrl+C to close the server or client safely.

- All sockets close properly, no resource leaks.

## Notes
- UDP is used for simplicity and performance. Message delivery is not guaranteed, but works well in local networks.

- Ideal for learning socket programming and game synchronization .

## Conclusion
This UDP Word Guessing Game offers a simple yet engaging way to learn network programming using sockets in C. With real-time updates, multiple client handling, and word-based logic, it’s a great project for practicing communication protocols and interactive gameplay logic.
