#ifndef MENU_CPP
#define MENU_CPP

#include <iostream>

using namespace std;

// Menu
void mostrarMenu() {
    cout << "\n--- MENU ---\n";
    cout << "1. Leer dataset (CSV)\n";
    cout << "2. Mostrar numero de registros\n";
    cout << "3. Mostrar numero de variables\n";
    cout << "4. Reportar valores no nulos\n";
    cout << "5. Reportar tipo de datos (por columna)\n";
    cout << "6. Reportar conteo total por tipo\n";
    cout << "0. Salir\n";
    cout << "Seleccione una opcion: ";
}

#endif
