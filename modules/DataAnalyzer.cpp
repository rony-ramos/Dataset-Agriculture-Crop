#ifndef DATAANALYZER_CPP
#define DATAANALYZER_CPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include "DatasetReader.cpp"
#include "Queue.cpp"
#include "HashTable.cpp"
#include "BST.cpp"
#include "Stack.cpp"

using namespace std;

// Clase para análisis de datos
class DataAnalyzer {
private:
    const DatasetReader& reader;

    bool is_numeric(const string& s) const {
        if (s.empty()) return false;
        try { stod(s); } catch(...) { return false; }
        return true;
    }

public:
    DataAnalyzer(const DatasetReader& r) : reader(r) {}

    size_t getNumeroRegistros() const { return reader.rows().size(); }
    size_t getNumeroVariables() const { return reader.header().size(); }

    void reportarValoresNoNulos() const {
        cout << "\n[OPCION 4] Cantidad de valores no nulos por columna:" << endl;
        vector<string> headers = reader.header();
        vector<size_t> nonNullCounts(headers.size(), 0);

        reader.rows().for_each([&](const vector<string>& row){
            for (size_t i = 0; i < row.size() && i < headers.size(); ++i) {
                if (!row[i].empty()) nonNullCounts[i]++;
            }
        });

        for (size_t i = 0; i < headers.size(); ++i) {
            cout << "- " << left << setw(25) << headers[i] << ": " << nonNullCounts[i] << " valores" << endl;
        }
    }

    // Devuelve un vector con el tipo inferido por columna: "Numerico" o "Categorico"
    vector<string> getTiposColumnas() const {
        vector<string> headers = reader.header();
        vector<string> first_row_data;
        vector<string> types(headers.size(), "Categorico");
        if (!reader.rows().get(0, first_row_data)) return types; // todos categoricos por defecto si no hay datos

        for (size_t i = 0; i < headers.size(); ++i) {
            if (i < first_row_data.size() && is_numeric(first_row_data[i])) types[i] = "Numerico";
            else types[i] = "Categorico";
        }
        return types;
    }

    // Opción 5: reporta tipo por columna
    void reportarTiposDeDatos() const {
        cout << "\n[OPCION 5] Tipo de dato inferido por columna:" << endl;
        vector<string> headers = reader.header();
        vector<string> types = getTiposColumnas();
        for (size_t i = 0; i < headers.size(); ++i) {
            cout << "- " << left << setw(25) << headers[i] << ": " << types[i] << endl;
        }
    }

    // Opción 6: reporta conteo total de columnas por tipo
    void reportarConteoTipos() const {
        vector<string> types = getTiposColumnas();
        size_t numeric_count = 0, categoric_count = 0;
        for (const auto& t : types) {
            if (t == "Numerico") ++numeric_count;
            else ++categoric_count;
        }
        cout << "\n[OPCION 6] Cantidad total de columnas por tipo de dato:" << endl;
        cout << "- Columnas Numericas: " << numeric_count << endl;
        cout << "- Columnas Categoricas: " << categoric_count << endl;
    }

    // Opción 7: Contar frecuencia de cada valor de variable categórica
    void contarFrecuenciasCategoricas() const {
        vector<string> headers = reader.header();
        vector<string> types = getTiposColumnas();
        
        // Filtrar solo las variables categóricas
        vector<size_t> categoricIndices;
        cout << "\n[OPCION 7] Variables categoricas disponibles:" << endl;
        for (size_t i = 0; i < headers.size(); ++i) {
            if (types[i] == "Categorico") {
                categoricIndices.push_back(i);
                cout << categoricIndices.size() << ". " << headers[i] << endl;
            }
        }
        
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
        
        size_t colIndex = categoricIndices[selection - 1];
        string selectedVar = headers[colIndex];
        
        cout << "\nContando frecuencias para la variable: " << selectedVar << endl;
        cout << "Usando: Cola (nodos) para leer registros y Tabla Hash (separate chaining) para conteo" << endl;
        
        // Cargar todos los registros en una cola
        Queue<vector<string>> rowQueue;
        reader.rows().for_each([&](const vector<string>& row) {
            rowQueue.enqueue(row);
        });
        
        // Procesar la cola y contar con HashTable
        HashTable<string, int> frequencyTable(100);
        vector<string> row;
        
        while (rowQueue.dequeue(row)) {
            if (colIndex < row.size() && !row[colIndex].empty()) {
                frequencyTable.increment(row[colIndex]);
            }
        }
        
        // Recopilar y ordenar resultados
        vector<pair<string, int>> results;
        frequencyTable.forEach([&](const string& key, int value) {
            results.push_back({key, value});
        });
        
        sort(results.begin(), results.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second; // Ordenar por frecuencia descendente
             });
        
        // Mostrar resultados
        cout << "\n--- Resultados ---" << endl;
        cout << "Variable: " << selectedVar << endl;
        cout << "Valores unicos encontrados: " << results.size() << endl;
        cout << "\nFrecuencias:" << endl;
        cout << left << setw(30) << "Valor" << "| Frecuencia" << endl;
        cout << string(45, '-') << endl;
        
        for (const auto& pair : results) {
            cout << left << setw(30) << pair.first << "| " << pair.second << endl;
        }
    }

    // Opción 8: K Registros similares (más cercanos a un valor X en una variable numérica)
    void encontrarKRegistrosSimilares() const {
        vector<string> headers = reader.header();
        vector<string> types = getTiposColumnas();
        
        // Filtrar solo las variables numéricas
        vector<size_t> numericIndices;
        cout << "\n[OPCION 8] Variables numericas disponibles:" << endl;
        for (size_t i = 0; i < headers.size(); ++i) {
            if (types[i] == "Numerico") {
                numericIndices.push_back(i);
                cout << numericIndices.size() << ". " << headers[i] << endl;
            }
        }
        
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
        
        size_t colIndex = numericIndices[selection - 1];
        string selectedVar = headers[colIndex];
        
        cout << "Ingrese el valor X (referencia): ";
        double x;
        cin >> x;
        
        cout << "Ingrese el valor de K (cantidad de registros similares): ";
        size_t k;
        cin >> k;
        
        cout << "\nBuscando los " << k << " registros mas cercanos a " << x 
             << " en la variable: " << selectedVar << endl;
        cout << "Usando: BST para almacenar registros y Pila (nodos) para recuperar K mas cercanos" << endl;
        
        // Construir BST con los registros
        BST<vector<string>> bst;
        size_t validRows = 0;
        
        reader.rows().for_each([&](const vector<string>& row) {
            if (colIndex < row.size() && !row[colIndex].empty()) {
                try {
                    double value = stod(row[colIndex]);
                    bst.insert(value, row);
                    ++validRows;
                } catch (...) {
                    // Ignorar valores no numéricos
                }
            }
        });
        
        if (validRows == 0) {
            cout << "No se encontraron valores numericos validos en la columna seleccionada." << endl;
            return;
        }
        
        // Encontrar los K más cercanos
        vector<vector<string>> closest = bst.findKClosest(x, k);
        
        if (closest.empty()) {
            cout << "No se encontraron registros." << endl;
            return;
        }
        
        // Usar una pila para almacenar y luego mostrar
        Stack<vector<string>> resultStack;
        for (const auto& row : closest) {
            resultStack.push(row);
        }
        
        // Mostrar resultados
        cout << "\n--- Resultados ---" << endl;
        cout << "Se encontraron " << closest.size() << " registros similares:" << endl;
        cout << string(80, '-') << endl;
        
        // Imprimir encabezados
        for (size_t i = 0; i < headers.size(); ++i) {
            cout << left << setw(15) << headers[i];
            if (i < headers.size() - 1) cout << "| ";
        }
        cout << endl;
        cout << string(80, '-') << endl;
        
        // Desapilar y mostrar cada registro
        vector<string> row;
        while (resultStack.pop(row)) {
            for (size_t i = 0; i < row.size() && i < headers.size(); ++i) {
                string value = i < row.size() ? row[i] : "";
                if (value.length() > 14) value = value.substr(0, 11) + "...";
                cout << left << setw(15) << value;
                if (i < headers.size() - 1) cout << "| ";
            }
            cout << endl;
        }
    }
};

#endif
