#ifndef CASHIER_H
#define CASHIER_H

#include "DoublyLinkedList.h"
#include <atomic>
#include <mutex>

class Cashier {
public:
    int id;
    std::atomic<bool> cancel_flag;
    std::atomic<bool> stop_flag;

    Cashier(int cashier_id);

    void process(Client client, DoublyLinkedList& queue, std::mutex& queue_mutex);
};

#endif // CASHIER_H