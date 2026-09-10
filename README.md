# ITMO.GameDev / C++ Programming / Practice 02 - Shop Queue Game

## Project Description

This project is a C++ application that simulates a store queue system with multiple cashiers and clients. The program allows users to manage the queue from the command line and observe how cashiers process clients concurrently.

The application supports the following operations:
1. **Initialize the simulation**: Set the number of clients (5-15) and cashiers (1-5), or use random values.
2. **Process clients concurrently**: Cashiers serve clients in parallel, and service time depends on the client's item count with a small random variation.
3. **Add a new cashier**: Use the `marina` command to open an additional cashier during the simulation.
4. **Cancel the current client**: Use `galya <id>` to interrupt the specified cashier and return their current client to the front of the queue.
5. **Add a new client**: Use the `add` command to append a client with a random number of items (1-15) to the queue.
6. **Stop a cashier**: Use `obed <id>` to stop the specified cashier and return their current client to the queue, as long as at least one cashier remains active.

The queue is implemented as a custom doubly linked list. The container itself is not thread-safe; thread safety is provided externally in `GameManager` via a mutex that guards shared queue access.

## Build Instructions

1. Ensure CMake and a C++17-compatible compiler (e.g., g++) are installed.
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
   ./ShopQueue
   ```

## Usage

Run the program and follow the prompts:
- Enter the number of clients (5-15 or `0` for random).
- Enter the number of cashiers (1-5 or `0` for random).
- Once the simulation starts, use the following commands:
  - `marina`: Add a new cashier.
  - `galya <id>`: Cancel the current client at cashier `<id>`.
  - `add`: Add a new client to the end of the queue.
  - `obed <id>`: Stop cashier `<id>` if at least one other cashier remains active.

The program finishes when all clients have been served and prints `All clients served. Game over.`
