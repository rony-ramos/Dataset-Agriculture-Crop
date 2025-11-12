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
    
    // Merge Sort (O(N log N)) - Algoritmo visto en clase
    void mergeSortPairs(Pair* arr, int left, int right) const {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        mergeSortPairs(arr, left, mid);
        mergeSortPairs(arr, mid + 1, right);
        
        // Merge
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        Pair* L = new Pair[n1];
        Pair* R = new Pair[n2];
        
        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];
        
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i].key <= R[j].key) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
        
        delete[] L;
        delete[] R;
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

    // Encontrar K elementos más cercanos con MERGE SORT (O(N log N))
    void findKClosest(double x, size_t k, LinkedList<T>& result) const {
        LinkedList<Pair> allElements;
        inorder(allElements);
        
        if (allElements.empty()) return;
        
        size_t n = allElements.size();
        
        // Convertir a arreglo para ordenar
        Pair* arr = new Pair[n];
        size_t idx = 0;
        allElements.for_each([&](const Pair& elem) {
            arr[idx].key = fabs(elem.key - x);  // Distancia a X
            arr[idx].data = elem.data;
            idx++;
        });
        
        // Merge Sort por distancia
        mergeSortPairs(arr, 0, n - 1);
        
        // Seleccionar los K más cercanos
        size_t limit = (k < n) ? k : n;
        for (size_t i = 0; i < limit; ++i) {
            result.push_back(arr[i].data);
        }
        
        delete[] arr;
    }

    const char* getName() const { return "BST (Arbol Binario Simple)"; }
    size_t size() const { return count; }
    bool isEmpty() const { return root == nullptr; }
};

#endif
