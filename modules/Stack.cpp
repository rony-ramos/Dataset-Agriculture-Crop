#ifndef STACK_CPP
#define STACK_CPP

#include <cstddef>

// Pila basada en nodos (para la opción 8)
template<typename T>
class Stack {
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* top;
    size_t count;

public:
    Stack() : top(nullptr), count(0) {}
    
    ~Stack() {
        while (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        ++count;
    }

    bool pop(T& out) {
        if (!top) return false;
        Node* temp = top;
        out = top->data;
        top = top->next;
        delete temp;
        --count;
        return true;
    }

    bool peek(T& out) const {
        if (!top) return false;
        out = top->data;
        return true;
    }

    bool isEmpty() const { return top == nullptr; }
    size_t size() const { return count; }
};

#endif
