#include "DoublyLinkedList.h"

#include <stdexcept>

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), list_size(0) {}
DoublyLinkedList::~DoublyLinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void DoublyLinkedList::push_back(const Client& client) {
    Node* newNode = new Node(client);

    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    ++list_size;
}

void DoublyLinkedList::push_front(const Client& client) {
    Node* newNode = new Node(client);

    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    ++list_size;
}

Client DoublyLinkedList::pop_front() {
    if (!head) {
        throw std::runtime_error("List is empty");
    }

    Client data = head->data;
    Node* temp = head;
    head = head->next;

    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }

    delete temp;
    --list_size;

    return data;
}

bool DoublyLinkedList::empty() const {
    return list_size == 0;
}

size_t DoublyLinkedList::size() const {
    return list_size;
}