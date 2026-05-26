#include "Client.h"

Client::Client(int client_id, int num_items) : id(client_id), items(num_items) {}

int Client::get_id() const {
    return id;
}

int Client::get_items() const {
    return items;
}
