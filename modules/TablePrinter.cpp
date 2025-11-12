#ifndef TABLEPRINTER_CPP
#define TABLEPRINTER_CPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include "LinkedList.cpp"

using namespace std;

// Imprime tabla con '|' y líneas horizontales usando guiones
class TablePrinter {
    vector<string> hdr;
    const LinkedList<vector<string>>& rowsRef;
    size_t maxRows;
public:
    TablePrinter(const vector<string>& header, const LinkedList<vector<string>>& rows, size_t maxR = 10)
        : hdr(header), rowsRef(rows), maxRows(maxR) {}

    void print() const {
        // Primera pasada: calcular número de columnas y anchos máximos sin copiar filas
        size_t cols = hdr.size();
        vector<size_t> widths(cols, 0);
        for (size_t c = 0; c < hdr.size(); ++c) widths[c] = hdr[c].size();

        rowsRef.for_first(maxRows, [&](const vector<string>& r){
            if (r.size() > cols) {
                size_t old = cols;
                widths.resize(r.size());
                for (size_t i = old; i < r.size(); ++i) widths[i] = 0;
                cols = r.size();
            }
            for (size_t c = 0; c < r.size(); ++c) widths[c] = max(widths[c], r[c].size());
        });

        // función para imprimir separador horizontal usando guiones y '|' como bordes verticales
        auto print_sep = [&]() {
            cout << "|";
            for (size_t c = 0; c < cols; ++c) cout << string(widths[c] + 2, '-') << "|";
            cout << "\n";
        };

        // imprimir cabecera
        print_sep();
        cout << "|";
        for (size_t c = 0; c < cols; ++c) {
            string cell = c < hdr.size() ? hdr[c] : string();
            cout << ' ' << left << setw((int)widths[c]) << cell << ' ' << "|";
        }
        cout << "\n";
        print_sep();

        // Segunda pasada: imprimir filas usando los anchos calculados
        rowsRef.for_first(maxRows, [&](const vector<string>& r){
            cout << "|";
            for (size_t c = 0; c < cols; ++c) {
                string cell = c < r.size() ? r[c] : string();
                cout << ' ' << left << setw((int)widths[c]) << cell << ' ' << "|";
            }
            cout << "\n";
            
        });
        print_sep();
    }
};

#endif
