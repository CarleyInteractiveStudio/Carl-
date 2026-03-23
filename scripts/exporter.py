import torch
import struct
import json
import os
from model.model import CarlModel, CarlConfig

def export_to_ccia(pt_model_path, config, output_path, quantization="fp32"):
    """
    Exporta un modelo de PyTorch al formato personalizado .ccia
    Estructura .ccia:
    - Magic Number (4 bytes): 'CCIA'
    - Versión (4 bytes): int
    - Precisión (4 bytes): 0=FP32, 1=FP16, 2=INT8
    - Vocab Size (4 bytes): int
    - Embed Dim (4 bytes): int
    - N Heads (4 bytes): int
    - N Layers (4 bytes): int
    - Block Size (4 bytes): int
    - Pesos (Binario)
    """

    # Cargar modelo
    device = "cpu"
    model = CarlModel(config)
    model.load_state_dict(torch.load(pt_model_path, map_location=device))
    model.eval()

    precision_code = 0
    if quantization == "fp16": precision_code = 1
    elif quantization == "int8": precision_code = 2

    # Obtener todos los parámetros en orden determinista
    state_dict = model.state_dict()
    param_names = list(state_dict.keys())

    with open(output_path, 'wb') as f:
        # Header
        f.write(b'CCIA')
        f.write(struct.pack('i', 1)) # Versión
        f.write(struct.pack('i', precision_code))
        f.write(struct.pack('i', config.vocab_size))
        f.write(struct.pack('i', config.n_embd))
        f.write(struct.pack('i', config.n_head))
        f.write(struct.pack('i', config.n_layer))
        f.write(struct.pack('i', config.block_size))
        f.write(struct.pack('i', len(param_names))) # Cantidad de tensores

        # Pesos
        for name in param_names:
            param = state_dict[name]
            data = param.detach().cpu().numpy()

            # Metadata del tensor
            name_bytes = name.encode('utf-8')
            f.write(struct.pack('i', len(name_bytes)))
            f.write(name_bytes)
            f.write(struct.pack('i', len(data.shape)))
            for dim in data.shape:
                f.write(struct.pack('i', dim))

            if quantization == "fp32":
                data = data.astype('float32')
            elif quantization == "fp16":
                data = data.astype('float16')

            f.write(data.tobytes())

    print(f"Modelo exportado exitosamente a {output_path} en formato {quantization}")

if __name__ == "__main__":
    # Cargar config real usada en el entrenamiento
    config = CarlConfig(
        vocab_size=5000,
        n_embd=256,
        n_head=8,
        n_layer=6,
        block_size=32
    )

    os.makedirs("weights", exist_ok=True)
    export_to_ccia("model/weights/carl_v0.1.pt", config, "weights/carl_v0.1.ccia", quantization="fp32")
