#ifndef CARL_NEURONAL_H
#define CARL_NEURONAL_H

#include <vector>

// Estructura para el Cerebro
struct CarlTensorNeuronal {
    std::vector<int> shape;
    std::vector<float> data;
    std::vector<float> grad;
    bool requires_grad;

    CarlTensorNeuronal(std::vector<int> s, bool req_grad = false);
    void zero_grad();
};

// Declaración de funciones del Cerebro (C-Interface)
extern "C" {
    void cerebro_matmul(const float* a, const float* b, float* c, int M, int K, int N);
    void cerebro_matmul_backward(const float* a, const float* b, const float* grad_c,
                                 float* grad_a, float* grad_b,
                                 int M, int K, int N);
    void cerebro_gelu(float* data, int size);
    void cerebro_layernorm(float* x, const float* gamma, const float* beta, int B, int C, float eps);
}

#endif
