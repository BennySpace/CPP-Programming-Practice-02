#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
    int id;
    int items;

    Client(int client_id, int num_items);
};

#endif // CLIENT_H