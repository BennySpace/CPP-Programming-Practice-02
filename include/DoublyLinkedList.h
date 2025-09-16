#ifndef SHOPQUEUE_DOUBLY_LINKED_LIST_H
#define SHOPQUEUE_DOUBLY_LINKED_LIST_H

#include "Client.h"
#include <stdexcept>

class DoublyLinkedList {
    struct Node {
        Client data;
        Node* prev;
        Node* next;
        Node(const Client& client) : data(client), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t list_size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void push_back(const Client& c);
    void push_front(const Client& c);
    Client pop_front();
    bool empty() const;
    size_t size() const;
};

#endif // SHOPQUEUE_DOUBLY_LINKED_LIST_H