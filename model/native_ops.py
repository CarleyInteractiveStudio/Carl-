import ctypes
import os
import torch
import torch.nn as nn

# Intentar cargar el Cerebro de Carl (Motor C++)
lib_path = os.path.abspath("./engine/build/libcarl_neuronal.so")

carl_lib = None
if os.path.exists(lib_path):
    try:
        carl_lib = ctypes.CDLL(lib_path)
        # Configurar Conexiones Neuronales (Matmul Forward)
        carl_lib.cerebro_matmul.argtypes = [
            ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float),
            ctypes.c_int, ctypes.c_int, ctypes.c_int
        ]
        # Configurar Conexiones Neuronales (Matmul Backward)
        carl_lib.cerebro_matmul_backward.argtypes = [
            ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float),
            ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float),
            ctypes.c_int, ctypes.c_int, ctypes.c_int
        ]
        # Configurar Activación GELU
        carl_lib.cerebro_gelu.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
        # Configurar LayerNorm
        carl_lib.cerebro_layernorm.argtypes = [
            ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float),
            ctypes.c_int, ctypes.c_int, ctypes.c_float
        ]
    except Exception as e:
        print(f"Error cargando el Cerebro (C++): {e}")

if carl_lib is None:
    print("Aviso: Cerebro Neuronal (C++) no encontrado. Usando modo simulación en Python.")

def get_ptr(tensor):
    # Asegurar Float32 y CPU para ctypes
    t = tensor.detach().cpu().contiguous()
    return ctypes.cast(t.data_ptr(), ctypes.POINTER(ctypes.c_float))

class CarlNeuronalOps(torch.autograd.Function):
    """Capa de enlace entre el director (Python) y el Cerebro (C++)."""

    @staticmethod
    def forward(ctx, a, b):
        if carl_lib is None: return a @ b

        ctx.save_for_backward(a, b)
        M, K = a.shape
        K2, N = b.shape
        c = torch.zeros((M, N), device='cpu', dtype=torch.float32)

        carl_lib.cerebro_matmul(get_ptr(a), get_ptr(b), get_ptr(c), M, K, N)
        return c.to(a.device)

    @staticmethod
    def backward(ctx, grad_output):
        a, b = ctx.saved_tensors
        M, K = a.shape
        K2, N = b.shape

        if carl_lib is not None:
            grad_a = torch.zeros_like(a).cpu().contiguous()
            grad_b = torch.zeros_like(b).cpu().contiguous()
            carl_lib.cerebro_matmul_backward(
                get_ptr(a), get_ptr(b), get_ptr(grad_output),
                get_ptr(grad_a), get_ptr(grad_b),
                M, K, N
            )
            return grad_a.to(a.device), grad_b.to(b.device)
        else:
            grad_a = grad_output @ b.t()
            grad_b = a.t() @ grad_output
            return grad_a, grad_b

def carl_matmul(a, b):
    # Solo usar C++ para matrices 2D grandes para evitar overhead
    if a.dim() == 2 and b.dim() == 2:
        return CarlNeuronalOps.apply(a, b)
    return a @ b

def carl_gelu(x):
    if carl_lib is None: return torch.nn.functional.gelu(x)
    x_cpu = x.detach().cpu().contiguous()
    carl_lib.cerebro_gelu(get_ptr(x_cpu), x_cpu.numel())
    return x_cpu.to(x.device)

def carl_layernorm(x, gamma, beta, eps=1e-5):
    if carl_lib is None: return torch.nn.functional.layer_norm(x, (x.shape[-1],), gamma, beta, eps)
    orig_shape = x.shape
    x_flat = x.view(-1, x.shape[-1]).detach().cpu().contiguous()
    B, C = x_flat.shape
    carl_lib.cerebro_layernorm(get_ptr(x_flat), get_ptr(gamma), get_ptr(beta), B, C, eps)
    return x_flat.view(orig_shape).to(x.device)
