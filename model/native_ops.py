import ctypes
import numpy as np
import os
import torch
import torch.nn as nn

# Cargar el Cerebro de Carl (Motor C++)
lib_path = "./engine/build/libcarl_neuronal.so"
if os.path.exists(lib_path):
    carl_lib = ctypes.CDLL(lib_path)
    # Configurar Conexiones Neuronales
    carl_lib.cerebro_matmul.argtypes = [
        ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float),
        ctypes.c_int, ctypes.c_int, ctypes.c_int
    ]
else:
    carl_lib = None
    print("Aviso: Cerebro Neuronal (C++) no encontrado. Usando modo simulación en Python.")

class CarlNeuronalOps(torch.autograd.Function):
    """Capa de enlace entre el director de orquesta (Python) y el Cerebro de Carl (C++)."""

    @staticmethod
    def forward(ctx, a, b):
        if carl_lib is None: return a @ b

        ctx.save_for_backward(a, b)
        M, K = a.shape
        K2, N = b.shape
        c = torch.zeros((M, N), device=a.device)

        # Llamar al Cerebro en C++
        carl_lib.cerebro_matmul(
            a.contiguous().data_ptr(),
            b.contiguous().data_ptr(),
            c.data_ptr(),
            M, K, N
        )
        return c

    @staticmethod
    def backward(ctx, grad_output):
        a, b = ctx.saved_tensors
        M, K = a.shape
        K2, N = b.shape

        grad_a = torch.zeros_like(a)
        grad_b = torch.zeros_like(b)

        if carl_lib is not None:
            # En una implementación completa aquí llamaríamos a native_matmul_backward
            # Para esta versión usamos la potencia de autograd para simplificar
            grad_a = grad_output @ b.t()
            grad_b = a.t() @ grad_output

        return grad_a, grad_b

def carl_matmul(a, b):
    return CarlNeuronalOps.apply(a, b)
