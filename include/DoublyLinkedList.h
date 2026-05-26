#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

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
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
    DoublyLinkedList(DoublyLinkedList&&) = delete;
    DoublyLinkedList& operator=(DoublyLinkedList&&) = delete;

    void push_back(const Client& c);
    void push_front(const Client& c);
    Client pop_front();
    bool empty() const;
    size_t size() const;
};

#endif // DOUBLY_LINKED_LIST_H
