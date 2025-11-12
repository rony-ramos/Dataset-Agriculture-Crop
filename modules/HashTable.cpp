#ifndef HASHTABLE_CPP
#define HASHTABLE_CPP

#include <string>
#include <functional>
#include <vector>

using namespace std;

// Tabla Hash con separate chaining (para la opción 7)
template<typename K, typename V>
class HashTable {
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    vector<Entry*> table;
    size_t capacity;
    size_t count;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

public:
    HashTable(size_t cap = 100) : capacity(cap), count(0) {
        table.resize(capacity, nullptr);
    }

    ~HashTable() {
        for (size_t i = 0; i < capacity; ++i) {
            Entry* current = table[i];
            while (current) {
                Entry* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const K& key, const V& value) {
        size_t idx = hash(key);
        Entry* current = table[idx];
        
        // Buscar si la clave ya existe
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Insertar nueva entrada al inicio de la cadena
        Entry* newEntry = new Entry(key, value);
        newEntry->next = table[idx];
        table[idx] = newEntry;
        ++count;
    }

    bool find(const K& key, V& out) const {
        size_t idx = hash(key);
        Entry* current = table[idx];
        
        while (current) {
            if (current->key == key) {
                out = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void increment(const K& key) {
        size_t idx = hash(key);
        Entry* current = table[idx];
        
        while (current) {
            if (current->key == key) {
                ++current->value;
                return;
            }
            current = current->next;
        }
        
        // Si no existe, crear con valor 1
        Entry* newEntry = new Entry(key, 1);
        newEntry->next = table[idx];
        table[idx] = newEntry;
        ++count;
    }

    template<typename F>
    void forEach(F func) const {
        for (size_t i = 0; i < capacity; ++i) {
            Entry* current = table[i];
            while (current) {
                func(current->key, current->value);
                current = current->next;
            }
        }
    }

    size_t size() const { return count; }
};

#endif
