#ifndef CARL_NEURONAL_H
#define CARL_NEURONAL_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

// Cerebro de Carl: Estructura de Tensor con soporte para Gradientes
struct CarlTensorNeuronal {
    std::vector<int> shape;
    std::vector<float> data;
    std::vector<float> grad;
    bool requires_grad;

    CarlTensorNeuronal(std::vector<int> s, bool req_grad = false) : shape(s), requires_grad(req_grad) {
        int size = 1;
        for (int d : shape) size *= d;
        data.resize(size, 0.0f);
        if (requires_grad) grad.resize(size, 0.0f);
    }

    void zero_grad() {
        if (requires_grad) std::fill(grad.begin(), grad.end(), 0.0f);
    }
};

// El Cerebro de Carl: Operaciones matemáticas fundamentales (Neuronal Core)
extern "C" {
    // Multiplicación de Matrices Neuronal: C = A * B
    void cerebro_matmul(const float* a, const float* b, float* c, int M, int K, int N) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                float sum = 0;
                for (int k = 0; k < K; ++k) {
                    sum += a[i * K + k] * b[k * N + j];
                }
                c[i * N + j] = sum;
            }
        }
    }

    // Aprendizaje Neuronal: Backward de Matmul
    void cerebro_matmul_backward(const float* a, const float* b, const float* grad_c,
                                 float* grad_a, float* grad_b,
                                 int M, int K, int N) {
        // grad_a = grad_c * B^T
        for (int i = 0; i < M; ++i) {
            for (int k = 0; k < K; ++k) {
                float sum = 0;
                for (int j = 0; j < N; ++j) {
                    sum += grad_c[i * N + j] * b[k * N + j];
                }
                grad_a[i * K + k] += sum;
            }
        }
        // grad_b = A^T * grad_c
        for (int k = 0; k < K; ++k) {
            for (int j = 0; j < N; ++j) {
                float sum = 0;
                for (int i = 0; i < M; ++i) {
                    sum += a[i * K + k] * grad_c[i * N + j];
                }
                grad_b[k * N + j] += sum;
            }
        }
    }

    // Conexión Neuronal (GELU): Función de activación
    void cerebro_gelu(float* data, int size) {
        for (int i = 0; i < size; ++i) {
            float x = data[i];
            data[i] = 0.5f * x * (1.0f + tanhf(0.7978845608f * (x + 0.044715f * x * x * x)));
        }
    }
}

#endif
