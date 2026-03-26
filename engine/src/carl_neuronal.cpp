#include "carl_neuronal.h"
#include <cmath>
#include <algorithm>

// Implementación de la estructura
CarlTensorNeuronal::CarlTensorNeuronal(std::vector<int> s, bool req_grad) : shape(s), requires_grad(req_grad) {
    int size = 1;
    for (int d : shape) size *= d;
    data.resize(size, 0.0f);
    if (requires_grad) grad.resize(size, 0.0f);
}

void CarlTensorNeuronal::zero_grad() {
    if (requires_grad) std::fill(grad.begin(), grad.end(), 0.0f);
}

// Implementación de las funciones del Cerebro
extern "C" {
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

    void cerebro_matmul_backward(const float* a, const float* b, const float* grad_c,
                                 float* grad_a, float* grad_b,
                                 int M, int K, int N) {
        for (int i = 0; i < M; ++i) {
            for (int k = 0; k < K; ++k) {
                float sum = 0;
                for (int j = 0; j < N; ++j) {
                    sum += grad_c[i * N + j] * b[k * N + j];
                }
                grad_a[i * K + k] += sum;
            }
        }
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

    void cerebro_gelu(float* data, int size) {
        for (int i = 0; i < size; ++i) {
            float x = data[i];
            data[i] = 0.5f * x * (1.0f + tanhf(0.7978845608f * (x + 0.044715f * x * x * x)));
        }
    }

    void cerebro_layernorm(float* x, const float* gamma, const float* beta, int B, int C, float eps) {
        for (int i = 0; i < B; ++i) {
            float mean = 0, var = 0;
            for (int j = 0; j < C; ++j) mean += x[i * C + j];
            mean /= C;
            for (int j = 0; j < C; ++j) {
                float diff = x[i * C + j] - mean;
                var += diff * diff;
            }
            var /= C;
            float inv_std = 1.0f / sqrtf(var + eps);
            for (int j = 0; j < C; ++j) {
                x[i * C + j] = (x[i * C + j] - mean) * inv_std * gamma[j] + beta[j];
            }
        }
    }
}
