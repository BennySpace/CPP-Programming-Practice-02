#include "GameManager.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <chrono>
#include <thread>

#ifdef _WIN32
#define NOMINMAX
#include <conio.h>
#include <windows.h>
#endif

namespace {
bool has_pending_command() {
#ifdef _WIN32
    DWORD console_mode = 0;
    HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);

    if (input_handle != INVALID_HANDLE_VALUE && GetConsoleMode(input_handle, &console_mode)) {
        return _kbhit() != 0;
    }
#endif

    return std::cin.rdbuf()->in_avail() > 0;
}
}

void cashier_thread_func(std::shared_ptr<Cashier> cashier, DoublyLinkedList& queue, std::mutex& queue_mutex, std::atomic<int>& served) {
    while (!cashier->is_stop_requested()) {
        Client c{0, 0};
        bool has_client = false;

        {
            std::lock_guard<std::mutex> lock(queue_mutex);

            if (!queue.empty()) {
                c = queue.pop_front();
                has_client = true;
            }
        }

        if (!has_client) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        Cashier::ProcessResult result = cashier->process(c, queue, queue_mutex);

        if (result == Cashier::ProcessResult::Completed) {
            served.fetch_add(1);
            log_line("Cashier " + std::to_string(cashier->get_id()) + " incremented served to " +
                     std::to_string(served.load()));
        }
    }

    log_line("Cashier " + std::to_string(cashier->get_id()) + " has stopped.");
}

GameManager::GameManager() : served(0), total_clients(0), gen(std::random_device{}()) {
    std::uniform_int_distribution<int> dist_clients(5, 15);
    std::uniform_int_distribution<int> dist_cashiers(1, 5);
    std::uniform_int_distribution<int> dist_items(1, 15);

    num_clients = get_input_int("Enter number of clients (5-15) or 0 for random: ", 5, 15);

    if (num_clients == 0) {
        num_clients = dist_clients(gen);
    }

    log_line("Number of clients: " + std::to_string(num_clients));

    num_cashiers = get_input_int("Enter number of cashiers (1-5) or 0 for random: ", 1, 5);

    if (num_cashiers == 0) {
        num_cashiers = dist_cashiers(gen);
    }

    log_line("Number of cashiers: " + std::to_string(num_cashiers));

    initialize_clients();
    initialize_cashiers();
}

GameManager::~GameManager() {
    for (auto& cashier : cashiers) {
        cashier->request_stop();
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void GameManager::run() {
    for (auto& cashier : cashiers) {
        threads.emplace_back(cashier_thread_func, cashier, std::ref(queue), std::ref(queue_mutex), std::ref(served));
    }

    log_line("Game started. Commands: 'marina' (add cashier), 'galya <id>' (cancel), 'add' (add client), 'obed <id>' (stop cashier)");
    while (true) {
        bool queue_empty = false;

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            queue_empty = queue.empty();
        }

        if (served.load() >= total_clients.load() && queue_empty) {
            break;
        }

        if (!queue_empty && count_active_cashiers() == 0) {
            log_line("No active cashiers left. Game cannot continue.");
            break;
        }

        if (has_pending_command()) {
            std::string line;

            if (std::getline(std::cin, line) && !line.empty()) {
                handle_command(line);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    for (auto& cashier : cashiers) {
        cashier->request_stop();
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    log_line("All clients served. Game over.");
}

int GameManager::get_input_int(const std::string& prompt, int min_val, int max_val) {
    int value;
    std::cout << prompt;

    while (!(std::cin >> value) || (value != 0 && (value < min_val || value > max_val))) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number between " << min_val << " and " << max_val << " (or 0 for random): ";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return value;
}

void GameManager::initialize_clients() {
    std::uniform_int_distribution<int> dist_items(1, 15);
    total_clients.store(num_clients);

    for (int i = 1; i <= num_clients; ++i) {
        int items = dist_items(gen);
        queue.push_back(Client(i, items));
    }
}

void GameManager::initialize_cashiers() {
    for (int i = 1; i <= num_cashiers; ++i) {
        auto cashier = std::make_shared<Cashier>(i);
        cashiers.push_back(cashier);
    }
}

void GameManager::handle_command(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    std::uniform_int_distribution<int> dist_items(1, 15);

    if (command == "marina") {
        int new_id = cashiers.size() + 1;
        auto new_cashier = std::make_shared<Cashier>(new_id);
        cashiers.push_back(new_cashier);
        threads.emplace_back(cashier_thread_func, new_cashier, std::ref(queue), std::ref(queue_mutex), std::ref(served));
        log_line("Added new cashier " + std::to_string(new_id) + ".");
    } else if (command == "galya") {
        int cashier_id;
        bool found = false;

        if (iss >> cashier_id) {
            for (auto& cashier : cashiers) {
                if (cashier->get_id() == cashier_id) {
                    found = true;

                    if (cashier->request_cancel()) {
                        log_line("Cancel command sent to cashier " + std::to_string(cashier_id) + ".");
                    } else {
                        log_line("Cashier " + std::to_string(cashier_id) + " has no current client.");
                    }

                    break;
                }
            }

            if (!found) {
                log_line("Cashier " + std::to_string(cashier_id) + " not found.");
            }
        } else {
            log_line("Usage: galya <id>");
        }
    } else if (command == "add") {
        int new_id = total_clients.fetch_add(1) + 1;
        int items = dist_items(gen);

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            queue.push_back(Client(new_id, items));
        }

        log_line("Added new client " + std::to_string(new_id) + " to the queue.");
    } else if (command == "obed") {
        int cashier_id;
        bool found = false;

        if (iss >> cashier_id) {
            for (auto& cashier : cashiers) {
                if (cashier->get_id() == cashier_id) {
                    found = true;
                    if (count_active_cashiers() <= 1) {
                        log_line("Cannot stop cashier " + std::to_string(cashier_id) +
                                 " because it is the last active cashier.");
                        break;
                    }

                    cashier->request_stop();
                    log_line("Stop command sent to cashier " + std::to_string(cashier_id) + ".");

                    break;
                }
            }

            if (!found) {
                log_line("Cashier " + std::to_string(cashier_id) + " not found.");
            }
        } else {
            log_line("Usage: obed <id>");
        }
    } else if (!command.empty()) {
        log_line("Unknown command.");
    }
}

size_t GameManager::count_active_cashiers() const {
    size_t active_cashiers = 0;

    for (const auto& cashier : cashiers) {
        if (!cashier->is_stop_requested()) {
            ++active_cashiers;
        }
    }

    return active_cashiers;
}
