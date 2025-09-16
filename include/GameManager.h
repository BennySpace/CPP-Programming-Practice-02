#ifndef SHOPQUEUE_GAMEMANAGER_H
#define SHOPQUEUE_GAMEMANAGER_H

#include "DoublyLinkedList.h"
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
};

#endif //SHOPQUEUE_GAMEMANAGER_H