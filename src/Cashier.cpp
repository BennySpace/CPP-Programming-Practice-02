#include "Cashier.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

Cashier::Cashier(int cashier_id) : id(cashier_id), cancel_flag(false), stop_flag(false) {}

void Cashier::process(const Client& client, DoublyLinkedList& queue, std::mutex& queue_mutex) {
    std::cout << "Cashier " << id << " starting to serve client " << client.id << " with " << client.items << " items." << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_delay(3, 6);
    int delay_sec = dist_delay(gen);
    int total_ms = delay_sec * 1000;
    int step_ms = 100;

    for (int elapsed = 0; elapsed < total_ms; elapsed += step_ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));

        if (cancel_flag.load()) {
            std::cout << "Cancel triggered on cashier " << id << " for client " << client.id << ". Putting back to queue." << std::endl;

            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                queue.push_front(client);
            }

            cancel_flag.store(false);

            return;
        }
    }

    std::cout << "Cashier " << id << " finished serving client " << client.id << "." << std::endl;
}