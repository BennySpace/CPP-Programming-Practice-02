#include "Cashier.h"
#include "ConsoleUtils.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <thread>

Cashier::Cashier(int cashier_id) : id(cashier_id), service_state(ServiceState::Idle), stop_flag(false) {}

int Cashier::get_id() const {
    return id;
}

bool Cashier::is_stop_requested() const {
    return stop_flag.load();
}

bool Cashier::request_cancel() {
    ServiceState expected = ServiceState::Serving;
    return service_state.compare_exchange_strong(expected, ServiceState::CancelRequested);
}

void Cashier::request_stop() {
    stop_flag.store(true);
}

Cashier::ProcessResult Cashier::process(const Client& client, DoublyLinkedList& queue, std::mutex& queue_mutex) {
    service_state.store(ServiceState::Serving);

    log_line("Cashier " + std::to_string(id) + " starting to serve client " +
             std::to_string(client.get_id()) + " with " + std::to_string(client.get_items()) + " items.");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_extra_delay(0, 2);
    int delay_sec = std::min(6, 1 + (client.get_items() / 4) + dist_extra_delay(gen));
    int total_ms = delay_sec * 1000;
    int step_ms = 100;

    for (int elapsed = 0; elapsed < total_ms; elapsed += step_ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));

        if (service_state.load() == ServiceState::CancelRequested) {
            log_line("Cancel triggered on cashier " + std::to_string(id) + " for client " +
                     std::to_string(client.get_id()) + ". Putting back to queue.");

            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                queue.push_front(client);
            }

            service_state.store(ServiceState::Idle);
            return ProcessResult::Cancelled;
        }

        if (is_stop_requested()) {
            log_line("Stop triggered on cashier " + std::to_string(id) + " for client " +
                     std::to_string(client.get_id()) + ". Putting back to queue.");

            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                queue.push_front(client);
            }

            service_state.store(ServiceState::Idle);
            return ProcessResult::Stopped;
        }
    }

    if (service_state.exchange(ServiceState::Idle) == ServiceState::CancelRequested) {
        log_line("Cancel triggered on cashier " + std::to_string(id) + " for client " +
                 std::to_string(client.get_id()) + ". Putting back to queue.");

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            queue.push_front(client);
        }

        return ProcessResult::Cancelled;
    }

    log_line("Cashier " + std::to_string(id) + " finished serving client " + std::to_string(client.get_id()) + ".");
    return ProcessResult::Completed;
}
