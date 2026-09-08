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
    bool is_stop_requested() const;
    bool request_cancel();
    void request_stop();

    ProcessResult process(const Client& client, DoublyLinkedList& queue, std::mutex& queue_mutex);

private:
    enum class ServiceState {
        Idle,
        Serving,
        CancelRequested
    };

    int id;
    std::atomic<ServiceState> service_state;
    std::atomic<bool> stop_flag;
};

#endif // CASHIER_H
