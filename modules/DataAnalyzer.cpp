#ifndef DATAANALYZER_CPP
#define DATAANALYZER_CPP

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include "DatasetReader.cpp"
#include "Queue.cpp"
#include "HashTable.cpp"
#include "BST.cpp"
#include "AVL.cpp"
#include "Stack.cpp"

using namespace std;

// Clase para análisis de datos (usando solo TDA vistos en clase)
class DataAnalyzer {
private:
    const DatasetReader& reader;

    bool is_numeric(const string& s) const {
        if (s.empty()) return false;
        try { stod(s); } catch(...) { return false; }
        return true;
    }
    
    // Obtener tipos de columnas (usa LinkedList en lugar de vector)
    void getTiposColumnas(LinkedList<string>& types) const {
        const LinkedList<string>& headers = reader.header();
        Row firstRow;
        
        // Inicializar todos como "Categorico"
        headers.for_each([&](const string&) {
            types.push_back("Categorico");
        });
        
        // Si hay datos, verificar primera fila
        if (reader.rows().get(0, firstRow)) {
            size_t idx = 0;
            headers.for_each([&](const string&) {
                string field;
                if (firstRow.getField(idx, field) && is_numeric(field)) {
                    // Actualizar tipo a numérico
                    string* typePtr = types.getPtr(idx);
                    if (typePtr) *typePtr = "Numerico";
                }
                idx++;
            });
        }
    }

public:
    DataAnalyzer(const DatasetReader& r) : reader(r) {}

    size_t getNumeroRegistros() const { return reader.rows().size(); }
    size_t getNumeroVariables() const { return reader.header().size(); }

    void reportarValoresNoNulos() const {
        cout << "\n[OPCION 4] Cantidad de valores no nulos por columna:" << endl;
        const LinkedList<string>& headers = reader.header();
        
        // Usar LinkedList para conteos
        LinkedList<size_t> nonNullCounts;
        headers.for_each([&](const string&) {
            nonNullCounts.push_back(0);
        });

        reader.rows().for_each([&](const Row& row){
            size_t colIdx = 0;
            headers.for_each([&](const string&) {
                string field;
                if (row.getField(colIdx, field) && !field.empty()) {
                    size_t* countPtr = nonNullCounts.getPtr(colIdx);
                    if (countPtr) (*countPtr)++;
                }
                colIdx++;
            });
        });

        size_t idx = 0;
        headers.for_each([&](const string& header) {
            size_t count;
            nonNullCounts.get(idx, count);
            cout << "- " << left << setw(25) << header << ": " << count << " valores" << endl;
            idx++;
        });
    }

    // Opción 5: reporta tipo por columna
    void reportarTiposDeDatos() const {
        cout << "\n[OPCION 5] Tipo de dato inferido por columna:" << endl;
        const LinkedList<string>& headers = reader.header();
        LinkedList<string> types;
        getTiposColumnas(types);
        
        size_t idx = 0;
        headers.for_each([&](const string& header) {
            string type;
            types.get(idx, type);
            cout << "- " << left << setw(25) << header << ": " << type << endl;
            idx++;
        });
    }

    // Opción 6: reporta conteo total de columnas por tipo
    void reportarConteoTipos() const {
        LinkedList<string> types;
        getTiposColumnas(types);
        
        size_t numeric_count = 0, categoric_count = 0;
        types.for_each([&](const string& t) {
            if (t == "Numerico") ++numeric_count;
            else ++categoric_count;
        });
        
        cout << "\n[OPCION 6] Cantidad total de columnas por tipo de dato:" << endl;
        cout << "- Columnas Numericas: " << numeric_count << endl;
        cout << "- Columnas Categoricas: " << categoric_count << endl;
    }

    // Opción 7: Contar frecuencia de cada valor de variable categórica
    void contarFrecuenciasCategoricas() const {
        const LinkedList<string>& headers = reader.header();
        LinkedList<string> types;
        getTiposColumnas(types);
        
        // Filtrar solo las variables categóricas usando LinkedList
        LinkedList<size_t> categoricIndices;
        cout << "\n[OPCION 7] Variables categoricas disponibles:" << endl;
        
        size_t idx = 0;
        size_t displayNum = 1;
        headers.for_each([&](const string& header) {
            string type;
            types.get(idx, type);
            if (type == "Categorico") {
                categoricIndices.push_back(idx);
                cout << displayNum << ". " << header << endl;
                displayNum++;
            }
            idx++;
        });
        
        if (categoricIndices.empty()) {
            cout << "No hay variables categoricas en el dataset." << endl;
            return;
        }
        
        cout << "\nSeleccione el numero de la variable categorica: ";
        int selection;
        cin >> selection;
        
        if (selection < 1 || selection > (int)categoricIndices.size()) {
            cout << "Seleccion invalida." << endl;
            return;
        }
        
        size_t colIndex;
        categoricIndices.get(selection - 1, colIndex);
        
        string selectedVar;
        headers.get(colIndex, selectedVar);
        
        cout << "\nContando frecuencias para la variable: " << selectedVar << endl;
        cout << "Usando: Cola (nodos) para leer registros y Tabla Hash (separate chaining) para conteo" << endl;
        
        // Cargar todos los registros en una cola
        Queue<Row> rowQueue;
        reader.rows().for_each([&](const Row& row) {
            rowQueue.enqueue(row);
        });
        
        // Procesar la cola y contar con HashTable
        HashTable<string, int> frequencyTable(100);
        Row row;
        
        while (rowQueue.dequeue(row)) {
            string field;
            if (row.getField(colIndex, field) && !field.empty()) {
                frequencyTable.increment(field);
            }
        }
        
        // Recopilar resultados en LinkedList
        struct ResultPair {
            string key;
            int value;
            ResultPair() : value(0) {}
            ResultPair(const string& k, int v) : key(k), value(v) {}
        };
        
        LinkedList<ResultPair> results;
        frequencyTable.forEach([&](const string& key, int value) {
            results.push_back(ResultPair(key, value));
        });
        
        // Ordenar manualmente por frecuencia (Bubble Sort - visto en clase)
        size_t n = results.size();
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                ResultPair* p1 = results.getPtr(j);
                ResultPair* p2 = results.getPtr(j + 1);
                if (p1 && p2 && p1->value < p2->value) {
                    // Swap
                    ResultPair temp = *p1;
                    *p1 = *p2;
                    *p2 = temp;
                }
            }
        }
        
        // Mostrar resultados
        cout << "\n--- Resultados ---" << endl;
        cout << "Variable: " << selectedVar << endl;
        cout << "Valores unicos encontrados: " << results.size() << endl;
        cout << "\nFrecuencias:" << endl;
        cout << left << setw(30) << "Valor" << "| Frecuencia" << endl;
        cout << string(45, '-') << endl;
        
        results.for_each([](const ResultPair& pair) {
            cout << left << setw(30) << pair.key << "| " << pair.value << endl;
        });
    }

    // Opción 8: K Registros similares (más cercanos a un valor X en una variable numérica)
    void encontrarKRegistrosSimilares() const {
        const LinkedList<string>& headers = reader.header();
        LinkedList<string> types;
        getTiposColumnas(types);
        
        // Filtrar solo las variables numéricas usando LinkedList
        LinkedList<size_t> numericIndices;
        cout << "\n[OPCION 8] Variables numericas disponibles:" << endl;
        
        size_t idx = 0;
        size_t displayNum = 1;
        headers.for_each([&](const string& header) {
            string type;
            types.get(idx, type);
            if (type == "Numerico") {
                numericIndices.push_back(idx);
                cout << displayNum << ". " << header << endl;
                displayNum++;
            }
            idx++;
        });
        
        if (numericIndices.empty()) {
            cout << "No hay variables numericas en el dataset." << endl;
            return;
        }
        
        cout << "\nSeleccione el numero de la variable numerica: ";
        int selection;
        cin >> selection;
        
        if (selection < 1 || selection > (int)numericIndices.size()) {
            cout << "Seleccion invalida." << endl;
            return;
        }
        
        size_t colIndex;
        numericIndices.get(selection - 1, colIndex);
        
        string selectedVar;
        headers.get(colIndex, selectedVar);
        
        cout << "Ingrese el valor X (referencia): ";
        double x;
        cin >> x;
        
        cout << "Ingrese el valor de K (cantidad de registros similares): ";
        size_t k;
        cin >> k;
        
        // COMPARAR ESTRUCTURAS: BST vs AVL
        cout << "\n=== SELECCION DE ESTRUCTURA DE DATOS ===" << endl;
        cout << "1. BST (Arbol Binario Simple) - O(N log N) promedio, O(N^2) peor caso" << endl;
        cout << "2. AVL (Arbol Auto-Balanceado) + Arreglo - O(N log N) garantizado" << endl;
        cout << "Seleccione la estructura (1 o 2): ";
        int structChoice;
        cin >> structChoice;
        
        if (structChoice != 1 && structChoice != 2) {
            cout << "Seleccion invalida. Usando AVL por defecto." << endl;
            structChoice = 2;
        }
        
        cout << "\nBuscando los " << k << " registros mas cercanos a " << x 
             << " en la variable: " << selectedVar << endl;
        
        LinkedList<const Row*> closest;
        size_t validRows = 0;
        
        // Medir tiempo total (construcción + búsqueda)
        auto startTotal = chrono::high_resolution_clock::now();
        
        if (structChoice == 1) {
            // ===== OPCION 1: BST SIMPLE =====
            cout << "Estructura: BST (Arbol Binario Simple)" << endl;
            cout << "Algoritmo: Merge Sort O(N log N)" << endl;
            
            BST<const Row*> bst;
            
            // Construir BST
            auto startBuild = chrono::high_resolution_clock::now();
            reader.rows().for_each([&](const Row& row) {
                string field;
                if (row.getField(colIndex, field) && !field.empty()) {
                    try {
                        double value = stod(field);
                        bst.insert(value, &row);
                        ++validRows;
                    } catch (...) {}
                }
            });
            auto endBuild = chrono::high_resolution_clock::now();
            
            if (validRows == 0) {
                cout << "No se encontraron valores numericos validos." << endl;
                return;
            }
            
            // Buscar K más cercanos
            auto startSearch = chrono::high_resolution_clock::now();
            bst.findKClosest(x, k, closest);
            auto endSearch = chrono::high_resolution_clock::now();
            
            auto buildTime = chrono::duration_cast<chrono::milliseconds>(endBuild - startBuild);
            auto searchTime = chrono::duration_cast<chrono::milliseconds>(endSearch - startSearch);
            
            cout << "  - Tiempo construccion BST: " << buildTime.count() << " ms" << endl;
            cout << "  - Tiempo busqueda K-nearest: " << searchTime.count() << " ms" << endl;
            
        } else {
            // ===== OPCION 2: AVL CON ARREGLO =====
            cout << "Estructura: AVL (Arbol Auto-Balanceado) + Arreglo Dinamico" << endl;
            cout << "Algoritmo: Merge Sort O(N log N) + Rotaciones AVL" << endl;
            
            AVL<const Row*> avl;
            
            // Construir AVL
            auto startBuild = chrono::high_resolution_clock::now();
            reader.rows().for_each([&](const Row& row) {
                string field;
                if (row.getField(colIndex, field) && !field.empty()) {
                    try {
                        double value = stod(field);
                        avl.insert(value, &row);
                        ++validRows;
                    } catch (...) {}
                }
            });
            auto endBuild = chrono::high_resolution_clock::now();
            
            if (validRows == 0) {
                cout << "No se encontraron valores numericos validos." << endl;
                return;
            }
            
            // Buscar K más cercanos
            auto startSearch = chrono::high_resolution_clock::now();
            avl.findKClosest(x, k, closest);
            auto endSearch = chrono::high_resolution_clock::now();
            
            auto buildTime = chrono::duration_cast<chrono::milliseconds>(endBuild - startBuild);
            auto searchTime = chrono::duration_cast<chrono::milliseconds>(endSearch - startSearch);
            
            cout << "  - Tiempo construccion AVL: " << buildTime.count() << " ms" << endl;
            cout << "  - Tiempo busqueda K-nearest: " << searchTime.count() << " ms" << endl;
            cout << "  - Altura del arbol: " << avl.getTreeHeight() << endl;
        }
        
        auto endTotal = chrono::high_resolution_clock::now();
        auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTotal - startTotal);
        
        cout << "\n*** TIEMPO TOTAL: " << totalTime.count() << " ms ***" << endl;
        cout << "Registros validos procesados: " << validRows << endl;
        
        if (closest.empty()) {
            cout << "No se encontraron registros." << endl;
            return;
        }
        
        // Mostrar resultados
        cout << "\n--- Resultados ---" << endl;
        cout << "Se encontraron " << closest.size() << " registros similares:" << endl;
        cout << string(80, '-') << endl;
        
        // Imprimir encabezados
        idx = 0;
        headers.for_each([&](const string& header) {
            cout << left << setw(15) << header;
            if (idx < headers.size() - 1) cout << "| ";
            idx++;
        });
        cout << endl;
        cout << string(80, '-') << endl;
        
        // Mostrar cada registro (usando punteros - sin copias)
        closest.for_each([&](const Row* rowPtr) {
            idx = 0;
            headers.for_each([&](const string&) {
                string value;
                rowPtr->getField(idx, value);
                if (value.length() > 14) value = value.substr(0, 11) + "...";
                cout << left << setw(15) << value;
                if (idx < headers.size() - 1) cout << "| ";
                idx++;
            });
            cout << endl;
        });
    }
};

#endif
