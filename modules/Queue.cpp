#ifndef QUEUE_CPP
#define QUEUE_CPP

#include <cstddef>

// Cola basada en nodos (para la opción 7)
template<typename T>
class Queue {
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* front;
    Node* rear;
    size_t count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    
    ~Queue() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        ++count;
    }

    bool dequeue(T& out) {
        if (!front) return false;
        Node* temp = front;
        out = front->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        --count;
        return true;
    }

    bool peek(T& out) const {
        if (!front) return false;
        out = front->data;
        return true;
    }

    bool isEmpty() const { return front == nullptr; }
    size_t size() const { return count; }
};

#endif
