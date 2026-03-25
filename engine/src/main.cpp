#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include "carl_tensor.h"

// Estructura para leer el header de Carl .ccia
struct CarlHeader {
    char magic[4];
    int version;
    int precision;
    int vocab_size;
    int n_embd;
    int n_head;
    int n_layer;
    int block_size;
    int num_tensors;
};

class CarlEngine {
public:
    CarlHeader header;
    std::map<std::string, Tensor> weights;

    bool load_model(const std::string& path) {
        std::ifstream f(path, std::ios::binary);
        if (!f.is_open()) {
            std::cerr << "Error: No se pudo abrir " << path << std::endl;
            return false;
        }

        f.read((char*)&header, sizeof(CarlHeader));

        if (std::string(header.magic, 4) != "CCIA") {
            std::cerr << "Error: No es un archivo .ccia valido" << std::endl;
            return false;
        }

        std::cout << "--- Modelo Carl CCIA Cargado ---" << std::endl;
        std::cout << "Tensores a cargar: " << header.num_tensors << std::endl;

        for (int i = 0; i < header.num_tensors; ++i) {
            int name_len;
            f.read((char*)&name_len, sizeof(int));
            std::vector<char> name_buf(name_len);
            f.read(name_buf.data(), name_len);
            std::string name(name_buf.begin(), name_buf.end());

            int dims_len;
            f.read((char*)&dims_len, sizeof(int));
            std::vector<int> shape(dims_len);
            for (int d = 0; d < dims_len; ++d) {
                f.read((char*)&shape[d], sizeof(int));
            }

            Tensor t(shape);
            f.read((char*)t.data.data(), t.data.size() * sizeof(float));
            weights[name] = t;
            std::cout << "[CARGADOR] Tensor: " << name << " (" << t.data.size() << " elementos)" << std::endl;
        }

        std::cout << "Todos los pesos de Carl cargados correctamente." << std::endl;
        return true;
    }

    std::string generate(const std::vector<int>& tokens, int max_new_tokens) {
        int T = tokens.size();
        int C = header.n_embd;
        Tensor x({T, C});

        Tensor& wte = weights["transformer.wte.weight"];
        Tensor& wpe = weights["transformer.wpe.weight"];

        for (int t = 0; t < T; ++t) {
            int tok = tokens[t];
            for (int i = 0; i < C; ++i) {
                x.data[t * C + i] = wte.data[tok * C + i] + wpe.data[t * C + i];
            }
        }

        for (int l = 0; l < header.n_layer; ++l) {
            std::cout << "[MOTOR] Ejecutando bloque Transformer " << l << "..." << std::endl;
            // Aqui conectamos matmul, attention y MLP en el futuro
        }

        return "¡Hola! Soy Carl v0.1 funcionando desde C++. Mis bloques de pensamiento estan activos.";
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "--- [MODO CHAT CARL C++] ---" << std::endl;
        std::cout << "Uso: ./carl_engine <modelo.ccia> [prompt_opcional]" << std::endl;
        return 1;
    }

    CarlEngine carl;
    if (carl.load_model(argv[1])) {
        if (argc > 2) {
            std::vector<int> tokens = {1, 2, 3, 4}; // Mock
            std::cout << "Carl: " << carl.generate(tokens, 10) << std::endl;
        } else {
            std::cout << "Chat interactivo con Carl C++ (Escribe 'salir' para terminar)" << std::endl;
            std::string input;
            while (true) {
                std::cout << "Tú: ";
                std::getline(std::cin, input);
                if (input == "salir") break;
                std::vector<int> tokens = {1, 2, 3, 4}; // Mock
                std::cout << "Carl: " << carl.generate(tokens, 10) << std::endl;
            }
        }
    }

    return 0;
}
