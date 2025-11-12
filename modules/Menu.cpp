#ifndef MENU_CPP
#define MENU_CPP

#include <iostream>

using namespace std;

// Menu
void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "       ANALIZADOR DE DATASETS CSV       \n";
    cout << "========================================\n";
    cout << "1. Leer dataset (CSV)\n";
    cout << "2. Mostrar numero de registros\n";
    cout << "3. Mostrar numero de variables\n";
    cout << "4. Reportar valores no nulos\n";
    cout << "5. Reportar tipo de datos (por columna)\n";
    cout << "6. Reportar conteo total por tipo\n";
    cout << "7. Contar frecuencia de variable categorica\n";
    cout << "8. K Registros similares (con seleccion de algoritmo)\n";
    cout << "0. Salir\n";
    cout << "========================================\n";
    cout << "Seleccione una opcion: ";
}

#endif
