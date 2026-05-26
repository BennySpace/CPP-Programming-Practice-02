#ifndef CASHIER_H
#define CASHIER_H

#include "DoublyLinkedList.h"
#include <atomic>
#include <mutex>

class Cashier {
public:
    enum class ProcessResult {
        Completed,
        Cancelled,
        Stopped
    };

    Cashier(int cashier_id);

    int get_id() const;
    bool is_cancel_requested() const;
    bool is_stop_requested() const;
    void request_cancel();
    void clear_cancel();
    void request_stop();

    ProcessResult process(const Client& client, DoublyLinkedList& queue, std::mutex& queue_mutex);

private:
    int id;
    std::atomic<bool> cancel_flag;
    std::atomic<bool> stop_flag;
};

#endif // CASHIER_H
