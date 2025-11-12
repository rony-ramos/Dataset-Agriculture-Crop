#ifndef BST_CPP
#define BST_CPP

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// Árbol Binario de Búsqueda (para la opción 8)
template<typename T>
class BST {
public:
    struct Node {
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

    void inorderTraversal(Node* node, vector<pair<double, T>>& result) const {
        if (node) {
            inorderTraversal(node->left, result);
            result.push_back({node->key, node->data});
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

    // Obtener todos los elementos ordenados por clave
    vector<pair<double, T>> inorder() const {
        vector<pair<double, T>> result;
        inorderTraversal(root, result);
        return result;
    }

    // Encontrar los K elementos más cercanos a un valor X
    vector<T> findKClosest(double x, size_t k) const {
        vector<pair<double, T>> allElements = inorder();
        
        if (allElements.empty()) return vector<T>();
        
        // Calcular distancias y ordenar
        vector<pair<double, T>> withDistances;
        for (const auto& elem : allElements) {
            double distance = fabs(elem.first - x);
            withDistances.push_back({distance, elem.second});
        }
        
        // Ordenar por distancia
        sort(withDistances.begin(), withDistances.end(), 
             [](const pair<double, T>& a, const pair<double, T>& b) {
                 return a.first < b.first;
             });
        
        // Retornar los K más cercanos
        vector<T> result;
        size_t limit = min(k, withDistances.size());
        for (size_t i = 0; i < limit; ++i) {
            result.push_back(withDistances[i].second);
        }
        
        return result;
    }

    size_t size() const { return count; }
    bool isEmpty() const { return root == nullptr; }
};

#endif
