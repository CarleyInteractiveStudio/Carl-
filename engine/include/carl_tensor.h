#ifndef CARL_TENSOR_H
#define CARL_TENSOR_H

#include <vector>
#include <iostream>

struct Tensor {
    std::vector<int> shape;
    std::vector<float> data;

    Tensor() {}
    Tensor(std::vector<int> s) : shape(s) {
        int size = 1;
        for (int dim : shape) size *= dim;
        data.resize(size, 0.0f);
    }

    float& at(int i, int j) {
        return data[i * shape[1] + j];
    }
};

// Operaciones basicas de Carl (Matriz Multiplicacion "Pura" C++)
void matmul(const Tensor& a, const Tensor& b, Tensor& c) {
    int M = a.shape[0];
    int K = a.shape[1];
    int N = b.shape[1];

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0;
            for (int k = 0; k < K; ++k) {
                sum += a.data[i * K + k] * b.data[k * N + j];
            }
            c.data[i * N + j] = sum;
        }
    }
}

#endif
