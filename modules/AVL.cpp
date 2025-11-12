#ifndef AVL_CPP
#define AVL_CPP

#include <string>
#include <cmath>

using namespace std;

// Árbol AVL (auto-balanceado) con arreglo dinámico para resultados (TDA - visto en clase)
template<typename T>
class AVL {
public:
    struct Node {
        double key;
        T data;
        Node* left;
        Node* right;
        int height;
        
        Node(double k, const T& d) : key(k), data(d), left(nullptr), right(nullptr), height(1) {}
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

    int getHeight(Node* node) const {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(Node* node) {
        if (node) {
            int leftH = getHeight(node->left);
            int rightH = getHeight(node->right);
            node->height = 1 + (leftH > rightH ? leftH : rightH);
        }
    }

    // Rotación derecha
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }

    // Rotación izquierda
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }

    Node* insertNode(Node* node, double key, const T& data) {
        // 1. Inserción BST normal
        if (!node) {
            ++count;
            return new Node(key, data);
        }
        
        if (key < node->key)
            node->left = insertNode(node->left, key, data);
        else
            node->right = insertNode(node->right, key, data);
        
        // 2. Actualizar altura
        updateHeight(node);
        
        // 3. Obtener factor de balance
        int balance = getBalance(node);
        
        // 4. Casos de desbalance y rotaciones
        
        // Caso Left-Left
        if (balance > 1 && key < node->left->key)
            return rotateRight(node);
        
        // Caso Right-Right
        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);
        
        // Caso Left-Right
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Caso Right-Left
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }

    // Estructura para usar arreglo dinámico
    struct Pair {
        double key;
        T data;
        double distance;
        
        Pair() : key(0), data(T()), distance(0) {}
        Pair(double k, const T& d, double dist = 0) : key(k), data(d), distance(dist) {}
    };
    
    void inorderToArray(Node* node, Pair* arr, size_t& idx) const {
        if (!node) return;
        inorderToArray(node->left, arr, idx);
        arr[idx++] = Pair(node->key, node->data);
        inorderToArray(node->right, arr, idx);
    }
    
    // Merge Sort optimizado para arreglo (O(N log N))
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
            if (L[i].distance <= R[j].distance) {
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
    AVL() : root(nullptr), count(0) {}
    
    ~AVL() {
        destroyTree(root);
    }

    void insert(double key, const T& data) {
        root = insertNode(root, key, data);
    }

    // Encontrar K elementos más cercanos usando ARREGLO DINÁMICO (más rápido)
    template<typename ResultContainer>
    void findKClosest(double x, size_t k, ResultContainer& result) const {
        if (!root || count == 0) return;
        
        // 1. Crear arreglo dinámico para todos los elementos
        Pair* arr = new Pair[count];
        size_t idx = 0;
        
        // 2. Recorrido inorder directo a arreglo (O(N))
        inorderToArray(root, arr, idx);
        
        // 3. Calcular distancias (O(N))
        for (size_t i = 0; i < count; ++i) {
            arr[i].distance = fabs(arr[i].key - x);
        }
        
        // 4. Ordenar por distancia con Merge Sort (O(N log N))
        mergeSortPairs(arr, 0, count - 1);
        
        // 5. Seleccionar los K más cercanos (O(K))
        size_t limit = (k < count) ? k : count;
        for (size_t i = 0; i < limit; ++i) {
            result.push_back(arr[i].data);
        }
        
        delete[] arr;
    }

    const char* getName() const { return "AVL (Arbol Auto-Balanceado)"; }
    size_t size() const { return count; }
    bool isEmpty() const { return root == nullptr; }
    int getTreeHeight() const { return getHeight(root); }
};

#endif
