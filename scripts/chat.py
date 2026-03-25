import torch
import os
from tokenizer.tokenizer import CarlTokenizer
from model.model import CarlModel, CarlConfig

def run_carl_chat(pt_weights="model/weights/carl_v0.1.pt", vocab_path="tokenizer/carl_vocab.json"):
    # 1. Configuración de Hardware
    device = "cuda" if torch.cuda.is_available() else "cpu"
    print(f"--- Iniciando Chat con Carl en {device.upper()} ---")

    # 2. Cargar Tokenizador
    if not os.path.exists(vocab_path):
        print(f"Error: No se encontró el vocabulario en {vocab_path}. Entrena primero a Carl.")
        return
    tokenizer = CarlTokenizer()
    tokenizer.load(vocab_path)

    # 3. Cargar Arquitectura (Debe coincidir con la usada en el entrenamiento)
    config = CarlConfig(
        vocab_size=10000,
        n_embd=256,
        n_head=8,
        n_layer=6,
        block_size=64
    )

    model = CarlModel(config).to(device)

    # 4. Cargar Pesos
    if os.path.exists(pt_weights):
        model.load_state_dict(torch.load(pt_weights, map_location=device))
        print(f"Pesos de Carl cargados desde {pt_weights}")
    else:
        print(f"Aviso: No se encontraron pesos en {pt_weights}. Carl está usando un cerebro vacío (azar).")

    model.eval()

    # 5. Bucle de Chat
    print("\nEscribe 'salir' para terminar el chat.")
    while True:
        try:
            prompt = input("Tú: ").strip()
            if prompt.lower() in ["salir", "exit"]: break
            if not prompt: continue

            # Codificar
            input_ids = torch.tensor([tokenizer.encode(prompt)], dtype=torch.long).to(device)

            # Generar
            print("Carl está pensando...", end="\r")
            with torch.no_grad():
                # Carl genera hasta 50 nuevos tokens
                output_ids = model.generate(input_ids, max_new_tokens=50)

            # Decodificar
            response = tokenizer.decode(output_ids[0].tolist())

            # Limpiar el eco del prompt en la respuesta si es necesario
            # response = response[len(prompt):].strip()

            print(f"Carl: {response}")

        except KeyboardInterrupt:
            break
        except Exception as e:
            print(f"\nError en el chat: {e}")

if __name__ == "__main__":
    run_carl_chat()
