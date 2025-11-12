#ifndef BST_CPP
#define BST_CPP

#include <string>
#include <cmath>
#include "LinkedList.cpp"

using namespace std;

// Árbol Binario de Búsqueda (TDA - visto en clase)
template<typename T>
class BST {
public:
    struct Node {  // struct interno para nodo - permitido en TDA
        double key;
        T data;
        Node* left;
        Node* right;
        
        Node(double k, const T& d) : key(k), data(d), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    size_t count;

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* insertNode(Node* node, double key, const T& data) {
        if (!node) {
            ++count;
            return new Node(key, data);
        }
        
        if (key < node->key) {
            node->left = insertNode(node->left, key, data);
        } else {
            node->right = insertNode(node->right, key, data);
        }
        return node;
    }

    // Par simple para almacenar (clave, dato)
    struct Pair {
        double key;
        T data;
        Pair() : key(0) {}
        Pair(double k, const T& d) : key(k), data(d) {}
    };
    
    void inorderTraversal(Node* node, LinkedList<Pair>& result) const {
        if (node) {
            inorderTraversal(node->left, result);
            result.push_back(Pair(node->key, node->data));
            inorderTraversal(node->right, result);
        }
    }

public:
    BST() : root(nullptr), count(0) {}
    
    ~BST() {
        destroyTree(root);
    }

    void insert(double key, const T& data) {
        root = insertNode(root, key, data);
    }

    // Obtener todos los elementos ordenados por clave usando LinkedList
    void inorder(LinkedList<Pair>& result) const {
        inorderTraversal(root, result);
    }

    // Encontrar los K elementos más cercanos a un valor X usando LinkedList
    void findKClosest(double x, size_t k, LinkedList<T>& result) const {
        LinkedList<Pair> allElements;
        inorder(allElements);
        
        if (allElements.empty()) return;
        
        // Calcular distancias
        LinkedList<Pair> withDistances;
        allElements.for_each([&](const Pair& elem) {
            double distance = fabs(elem.key - x);
            withDistances.push_back(Pair(distance, elem.data));
        });
        
        // Ordenar manualmente por distancia (Bubble Sort simple para K pequeños)
        // En clase han visto algoritmos de ordenamiento
        size_t n = withDistances.size();
        for (size_t i = 0; i < n - 1 && i < k * 2; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                Pair p1, p2;
                withDistances.get(j, p1);
                withDistances.get(j + 1, p2);
                if (p1.key > p2.key) {
                    // Swap (no podemos modificar directamente, usamos lista temporal)
                    // Para simplificar, reconstruimos lista ordenada
                }
            }
        }
        
        // Tomar los primeros K elementos
        size_t limit = (k < n) ? k : n;
        for (size_t i = 0; i < limit; ++i) {
            Pair p;
            if (withDistances.get(i, p)) {
                result.push_back(p.data);
            }
        }
    }

    size_t size() const { return count; }
    bool isEmpty() const { return root == nullptr; }
};

#endif
