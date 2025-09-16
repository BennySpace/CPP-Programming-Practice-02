#ifndef SHOPQUEUE_GAMEMANAGER_H
#define SHOPQUEUE_GAMEMANAGER_H

#include "DoublyLinkedList.h"
#include "Cashier.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

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
    std::atomic<bool> served;
    std::atomic<bool> total_clients;
    std::vector<std::shared_ptr<Cashier>> cashiers;
    std::vector<std::thread> threads;
    std::mt19937 gen;

    void cashier_thread(std::shared_ptr<Cashier> cashier, int total_clients_val);
    void handle_command(const std::string& line);
    void initialize_clients();
    void initialize_cashiers();
    int get_input_int(const std::string& prompt, int min_val, int max_val);
};

#endif //SHOPQUEUE_GAMEMANAGER_H