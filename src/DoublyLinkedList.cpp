#include "DoublyLinkedList.h"

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), list_size(0) {}

DoublyLinkedList::~DoublyLinkedList() {
    Node* current = head;

    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void DoublyLinkedList::push_back(const Client& c) {
    Node* new_node = new Node(c);

    if (!head) {
        head = tail = new_node;
    } else {
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }

    ++list_size;
}

void DoublyLinkedList::push_front(const Client& c) {
    Node* new_node = new Node(c);

    if (!head) {
        head = tail = new_node;
    } else {
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
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