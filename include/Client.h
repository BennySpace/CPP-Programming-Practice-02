#ifndef SHOPQUEUE_CLIENT_H
#define SHOPQUEUE_CLIENT_H

class Client {
public:
    int id;
    int items;

    Client(int client_id, int num_items);
};

#endif // SHOPQUEUE_CLIENT_H