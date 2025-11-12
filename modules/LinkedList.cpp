#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP

#include <cstddef>

// Lista enlazada simple con plantillas (estructura de datos hecha en clase)
template<typename T>
class LinkedList {
    struct Node {
        T data;
        Node* next;
        Node(const T& d): data(d), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    size_t count;
public:
    LinkedList(): head(nullptr), tail(nullptr), count(0) {}
    ~LinkedList() {
        Node* cur = head;
        while (cur) { Node* tmp = cur; cur = cur->next; delete tmp; }
    }
    void push_back(const T& value) {
        Node* n = new Node(value);
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
        ++count;
    }
    size_t size() const { return count; }
    
    template<typename F>
    void for_each(F f) const {
        Node* cur = head;
        while (cur) { f(cur->data); cur = cur->next; }
    }
    
    template<typename F>
    void for_first(size_t n, F f) const {
        Node* cur = head;
        size_t i = 0;
        while (cur && i < n) { f(cur->data); cur = cur->next; ++i; }
    }
    
    bool get(size_t idx, T& out) const {
        Node* cur = head;
        size_t i = 0;
        while (cur) {
            if (i == idx) { out = cur->data; return true; }
            cur = cur->next; ++i;
        }
        return false;
    }
};

#endif
