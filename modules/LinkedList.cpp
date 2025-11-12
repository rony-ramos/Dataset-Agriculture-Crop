#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP

#include <cstddef>

// Lista enlazada simple con plantillas (TDA - visto en clase)
template<typename T>
class LinkedList {
    struct Node {  // struct para nodo interno - permitido en TDA
        T data;
        Node* next;
        Node(const T& d): data(d), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    size_t count;
public:
    LinkedList(): head(nullptr), tail(nullptr), count(0) {}
    
    // Constructor de copia
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* cur = other.head;
        while (cur) {
            push_back(cur->data);
            cur = cur->next;
        }
    }
    
    // Operador de asignación
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            // Limpiar actual
            Node* cur = head;
            while (cur) { Node* tmp = cur; cur = cur->next; delete tmp; }
            head = tail = nullptr;
            count = 0;
            
            // Copiar nuevo
            cur = other.head;
            while (cur) {
                push_back(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }
    
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
    
    void push_front(const T& value) {
        Node* n = new Node(value);
        if (!head) head = tail = n;
        else { n->next = head; head = n; }
        ++count;
    }
    
    bool pop_front(T& out) {
        if (!head) return false;
        Node* temp = head;
        out = head->data;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        --count;
        return true;
    }
    
    size_t size() const { return count; }
    bool empty() const { return count == 0; }
    
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
    
    // Método para obtener un elemento mutable (útil para modificaciones)
    T* getPtr(size_t idx) {
        Node* cur = head;
        size_t i = 0;
        while (cur) {
            if (i == idx) return &(cur->data);
            cur = cur->next; ++i;
        }
        return nullptr;
    }
    
    void clear() {
        Node* cur = head;
        while (cur) { Node* tmp = cur; cur = cur->next; delete tmp; }
        head = tail = nullptr;
        count = 0;
    }
};

#endif
