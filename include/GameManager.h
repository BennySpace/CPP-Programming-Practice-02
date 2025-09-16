#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Client.h"
#include "DoublyLinkedList.h"
#include "Cashier.h"
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <random>

class GameManager {
public:
    GameManager();
    ~GameManager();
    void run();

private:
    int num_clients;
    int num_cashiers;
    DoublyLinkedList queue;
    std::mutex queue_mutex;
    std::atomic<int> served;
    std::atomic<int> total_clients;
    std::vector<std::shared_ptr<Cashier>> cashiers;
    std::vector<std::thread> threads;
    std::mt19937 gen;

    void cashier_thread(std::shared_ptr<Cashier> cashier);
    void handle_command(const std::string& line);
    void initialize_clients();
    void initialize_cashiers();
    int get_input_int(const std::string& prompt, int min_val, int max_val);
};

#endif // GAME_MANAGER_H