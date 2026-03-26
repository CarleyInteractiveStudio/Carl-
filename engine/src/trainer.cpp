#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "carl_math.h"

int main() {
    std::cout << "--- Carl C++ Training Engine ---" << std::endl;
    std::cout << "Inicializando musculatura de bajo nivel..." << std::endl;

    // Prueba simple de matmul nativo
    float a[4] = {1, 2, 3, 4};
    float b[4] = {5, 6, 7, 8};
    float c[4] = {0, 0, 0, 0};

    native_matmul(a, b, c, 2, 2, 2);

    std::cout << "Matematica de Carl verificada: [" << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << "]" << std::endl;
    std::cout << "Listo para entrenamiento de alto rendimiento." << std::endl;

    return 0;
}
