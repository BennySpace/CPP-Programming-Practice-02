#include "GameManager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>

void cashier_thread_func(std::shared_ptr<Cashier> cashier, DoublyLinkedList& queue, std::mutex& queue_mutex, std::atomic<int>& served) {
    while (true) {
        if (cashier->stop_flag.load()) {
            std::cout << "Cashier " << cashier->id << " is stopping." << std::endl;
            break;
        }

        Client client{0, 0};
        bool has_client = false;

        {
            std::lock_guard<std::mutex> lock(queue_mutex);

            if (!queue.empty()) {
                client = queue.pop_front();
                has_client = true;
            }
        }

        if (!has_client) {
            break;
        }

        cashier->process(client, queue, queue_mutex);

        if (!cashier->cancel_flag.load()) {
            served.fetch_add(1);
        }

        if (cashier->stop_flag.load()) {
            std::cout << "Cashier " << cashier->id << " is stopping after current client." << std::endl;
            break;
        }
    }
}

GameManager::GameManager() : served(0), total_clients(0), gen(std::random_device{}()) {
    std::uniform_int_distribution<int> dist_clients(5, 15);
    std::uniform_int_distribution<int> dist_cashiers(1, 5);
    std::uniform_int_distribution<int> dist_items(1, 15);

    num_clients = get_input_int("Enter number of clients (5-15) or 0 for random: ", 0, 15);
    if (num_clients == 0) {
        num_clients = dist_clients(gen);
    } else if (num_clients < 5) {
        num_clients = 5;
    }
    std::cout << "Number of clients: " << num_clients << std::endl;

    num_cashiers = get_input_int("Enter number of cashiers (1-5) or 0 for random: ", 0, 5);
    if (num_cashiers == 0) {
        num_cashiers = dist_cashiers(gen);
    } else if (num_cashiers < 1) {
        num_cashiers = 1;
    }
    std::cout << "Number of cashiers: " << num_cashiers << std::endl;

    initialize_clients();
    initialize_cashiers();
}

GameManager::~GameManager() {
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

    std::cout << "Game started. Commands: 'marina' (add cashier), 'galya <id>' (cancel), 'newclient' (add client), 'close <id>' (stop cashier)" << std::endl;

    while (served.load() < total_clients.load()) {
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        handle_command(line);
    }

    std::cout << "All clients served. Game over." << std::endl;
}

int GameManager::get_input_int(const std::string& prompt, int min_val, int max_val) {
    int value;
    std::cout << prompt;

    while (!(std::cin >> value) || value < min_val || value > max_val) {
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
        std::cout << "Added new cashier " << new_id << "." << std::endl;
    } else if (command == "galya") {
        int cashier_id;

        if (iss >> cashier_id) {
            for (auto& cashier : cashiers) {
                if (cashier->id == cashier_id) {
                    cashier->cancel_flag.store(true);
                    std::cout << "Cancel command sent to cashier " << cashier_id << "." << std::endl;

                    break;
                }
            }
        }
    } else if (command == "newclient") {
        int new_id = total_clients.fetch_add(1) + 1;
        int items = dist_items(gen);

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            queue.push_back(Client(new_id, items));
        }

        std::cout << "Added new client " << new_id << " to the queue." << std::endl;
    } else if (command == "close") {
        int cashier_id;

        if (iss >> cashier_id) {
            for (auto& cashier : cashiers) {
                if (cashier->id == cashier_id) {
                    cashier->stop_flag.store(true);
                    std::cout << "Stop command sent to cashier " << cashier_id << "." << std::endl;

                    break;
                }
            }
        }
    }
}