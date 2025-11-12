#include <iostream>
#include <memory>
#include <limits>

// Importar módulos
#include "modules/LinkedList.cpp"
#include "modules/DatasetReader.cpp"
#include "modules/CSVReader.cpp"
#include "modules/TablePrinter.cpp"
#include "modules/DataAnalyzer.cpp"
#include "modules/Menu.cpp"

using namespace std;

int main() {
    unique_ptr<DatasetReader> reader;
    
    while (true) {
        mostrarMenu();
        int opt;
        
        while (!(cin >> opt)) {
            cout << "Entrada invalida. Ingrese un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Seleccione una opcion: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (opt == 0) break;

        if (opt > 1 && !reader) {
            cout << "\nERROR: Primero debe cargar el dataset (Opcion 1)." << endl;
            continue;
        }

        unique_ptr<DataAnalyzer> analyzer;
        if(reader) analyzer = make_unique<DataAnalyzer>(*reader);

        switch(opt) {
            case 1: {
                cout << "Ingrese la ruta del archivo CSV: ";
                string path;
                getline(cin, path);
                if (!path.empty() && path.front() == '"' && path.back() == '"') path = path.substr(1, path.size() - 2);
                
                reader = make_unique<CSVReader>();
                if (!reader->load(path)) {
                    cout << "No se pudo abrir el archivo: " << path << "\n";
                    reader.reset();
                } else {
                    cout << "Dataset cargado exitosamente.\n";
                    TablePrinter tp(reader->header(), reader->rows(), 10);
                    tp.print();
                }
                break;
            }
            case 2:
                cout << "\n[OPCION 2] El dataset tiene " << analyzer->getNumeroRegistros() << " registros." << endl;
                break;
            case 3:
                cout << "\n[OPCION 3] El dataset tiene " << analyzer->getNumeroVariables() << " variables." << endl;
                break;
            case 4:
                analyzer->reportarValoresNoNulos();
                break;
            case 5:
                analyzer->reportarTiposDeDatos();
                break;
            case 6:
                analyzer->reportarConteoTipos();
                break;
            case 7:
                analyzer->contarFrecuenciasCategoricas();
                break;
            case 8:
                analyzer->encontrarKRegistrosSimilares();
                break;
            default:
                cout << "Opcion invalida\n";
                break;
        }
    }
    cout << "Saliendo del programa..." << endl;
    return 0;
}