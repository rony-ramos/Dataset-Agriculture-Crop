#ifndef TABLEPRINTER_CPP
#define TABLEPRINTER_CPP

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include "LinkedList.cpp"
#include "Row.cpp"

using namespace std;

// Imprime tabla con '|' y líneas horizontales usando guiones
class TablePrinter {
    const LinkedList<string>& hdr;
    const LinkedList<Row>& rowsRef;
    size_t maxRows;
    
    // Calcula el ancho máximo para una columna
    size_t getMaxWidth(size_t colIndex) const {
        size_t maxWidth = 0;
        
        // Ancho del header
        string headerName;
        if (hdr.get(colIndex, headerName)) {
            maxWidth = headerName.length();
        }
        
        // Ancho de los datos
        size_t count = 0;
        rowsRef.for_first(maxRows, [&](const Row& row) {
            string field;
            if (row.getField(colIndex, field)) {
                if (field.length() > maxWidth) {
                    maxWidth = field.length();
                }
            }
            count++;
        });
        
        return maxWidth > 50 ? 50 : maxWidth; // Límite de ancho
    }
    
public:
    TablePrinter(const LinkedList<string>& header, const LinkedList<Row>& rows, size_t maxR = 10)
        : hdr(header), rowsRef(rows), maxRows(maxR) {}

    void print() const {
        size_t numCols = hdr.size();
        
        // Calcular anchos usando LinkedList
        LinkedList<size_t> widths;
        for (size_t i = 0; i < numCols; ++i) {
            widths.push_back(getMaxWidth(i));
        }
        
        // Función para imprimir separador horizontal
        auto print_sep = [&]() {
            cout << "+";
            widths.for_each([](size_t width) {
                cout << string(width + 2, '-') << "+";
            });
            cout << "\n";
        };
        
        cout << "\n";
        
        // Imprimir cabecera
        print_sep();
        cout << "|";
        size_t colIdx = 0;
        hdr.for_each([&](const string& colName) {
            size_t width;
            widths.get(colIdx, width);
            string truncated = colName.length() > width ? 
                             colName.substr(0, width - 3) + "..." : colName;
            cout << " " << left << setw(width) << truncated << " |";
            colIdx++;
        });
        cout << "\n";
        print_sep();
        
        // Imprimir filas
        size_t rowCount = 0;
        rowsRef.for_first(maxRows, [&](const Row& row) {
            cout << "|";
            colIdx = 0;
            size_t fieldIdx = 0;
            while (fieldIdx < numCols) {
                string field;
                row.getField(fieldIdx, field);
                size_t width;
                widths.get(fieldIdx, width);
                string truncated = field.length() > width ? 
                                 field.substr(0, width - 3) + "..." : field;
                cout << " " << left << setw(width) << truncated << " |";
                fieldIdx++;
            }
            cout << "\n";
            rowCount++;
        });
        print_sep();
        
        cout << "Mostrando " << rowCount << " de " << rowsRef.size() << " registros.\n\n";
    }
};

#endif
