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
    # Intentamos detectar el tamaño del vocabulario desde el archivo de pesos
    pt_path = "model/weights/carl_v0.1.pt"
    v_size = 10000
    b_size = 64

    if os.path.exists(pt_path):
        sd = torch.load(pt_path, map_location="cpu")
        if "transformer.wte.weight" in sd:
            v_size = sd["transformer.wte.weight"].shape[0]
        if "transformer.wpe.weight" in sd:
            b_size = sd["transformer.wpe.weight"].shape[0]

    # Intentamos detectar la dimensión de embedding
    e_dim = 512
    n_lay = 12
    if os.path.exists(pt_path):
        if "transformer.wte.weight" in sd:
            e_dim = sd["transformer.wte.weight"].shape[1]
        # Contar capas h.X.
        layers = set()
        for k in sd.keys():
            if k.startswith("transformer.h."):
                layers.add(k.split(".")[2])
        if layers:
            n_lay = len(layers)

    # Cargar config real usada en el entrenamiento
    config = CarlConfig(
        vocab_size=v_size,
        n_embd=e_dim,
        n_head=8,
        n_layer=n_lay,
        block_size=b_size
    )

    os.makedirs("weights", exist_ok=True)
    export_to_ccia("model/weights/carl_v0.1.pt", config, "weights/carl_v0.1.ccia", quantization="fp32")
