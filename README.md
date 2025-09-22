# ITMO.GameDev / C++ Programming / Practice 02 - Shop Queue Game

## Project Description

This project is a C++ application that simulates a store queue system with multiple cashiers and clients. The program allows users to manage a queue of clients being served by cashiers through a command-line interface. Key features include:

1. **Initialize Queue and Cashiers**: Set the number of clients (5–15) and cashiers (1–5), or use random values.
2. **Process Clients**: Cashiers serve clients with a random processing time (3–6 seconds per client), simulating item scanning.
3. **Interactive Commands**:
    - `marina`: Add a new cashier to the system.
    - `galya <id>`: Cancel the current client's processing at the specified cashier and return them to the queue.
    - `add`: Add a new client to the queue with a random number of items (1–15).
    - `obed <id>`: Stop the specified cashier, returning their current client to the queue.
4. **Thread-Safe Queue Management**: Uses a doubly-linked list with mutex protection for thread-safe queue operations.

The program uses multithreading to simulate concurrent cashier operations and ensures thread safety with mutexes and atomic variables.

## Build Instructions

1. Ensure CMake and a C++17-compatible compiler are installed.
2. Navigate to the project root directory.
3. Create and enter a build directory:
   ```bash
   mkdir build
   cd build
   ```
4. Run CMake and build the project:
   ```bash
   cmake ..
   cmake --build .
   ```
5. Run the executable:
   ```bash
   ./build/StoreQueueGame
   ```

## Usage

Run the program and follow the prompts:
- Enter the number of clients (5–15 or 0 for random).
- Enter the number of cashiers (1–5 or 0 for random).
- Once the simulation starts, input commands:
    - `marina`: Add a new cashier.
    - `galya <id>`: Cancel processing for cashier `<id>`.
    - `add`: Add a new client to the queue.
    - `obed <id>`: Stop cashier `<id>`.
- The program runs until all clients are served, then exits with "All clients served. Game over."