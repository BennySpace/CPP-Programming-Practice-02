#ifndef SHOPQUEUE_CASHIER_H
#define SHOPQUEUE_CASHIER_H

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


#endif //SHOPQUEUE_CASHIER_H