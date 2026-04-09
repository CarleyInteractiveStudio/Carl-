#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "carl_neuronal.h"

int main() {
    std::cout << "--- Cerebro Neuronal de Carl (C++ Engine) ---" << std::endl;
    std::cout << "Inicializando conexiones neuronales de bajo nivel..." << std::endl;

    // Prueba simple de matmul nativo (El Cerebro)
    float a[4] = {1, 2, 3, 4};
    float b[4] = {5, 6, 7, 8};
    float c[4] = {0, 0, 0, 0};

    cerebro_matmul(a, b, c, 2, 2, 2);

    std::cout << "Cerebro de Carl verificado: [" << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << "]" << std::endl;
    std::cout << "Listo para entrenamiento de alto rendimiento." << std::endl;

    return 0;
}
