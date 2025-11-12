#ifndef DATAANALYZER_CPP
#define DATAANALYZER_CPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "DatasetReader.cpp"

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
};

#endif
