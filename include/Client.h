#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
    Client(int client_id, int num_items);

    int get_id() const;
    int get_items() const;

private:
    int id;
    int items;
};

#endif // CLIENT_H
