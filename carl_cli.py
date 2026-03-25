import os
import sys
from scripts.book_downloader import CarlBookDownloader
from extractor.extractor import CarlDataExtractor
from trainer.train import train_carl
from scripts.exporter import export_to_ccia
from model.model import CarlConfig

def run_command(command_str):
    parts = command_str.split()
    if not parts: return
    cmd = parts[0]

    if cmd == "descargar":
        cantidad = int(parts[1]) if len(parts) > 1 else 10
        dl = CarlBookDownloader()
        dl.download_spanish_books(cantidad)

    elif cmd == "procesar":
        input_dir = parts[1] if len(parts) > 1 else "data/input"
        ex = CarlDataExtractor()
        ex.process_directory(input_dir)

    elif cmd == "entrenar":
        epochs = int(parts[1]) if len(parts) > 1 else 1
        print(f"Iniciando entrenamiento por {epochs} épocas...")
        train_carl()

    elif cmd == "exportar":
        precision = parts[1] if len(parts) > 1 else "fp32"
        config = CarlConfig(vocab_size=5000, n_embd=256, n_head=8, n_layer=6, block_size=32)
        os.makedirs("weights", exist_ok=True)
        export_to_ccia("model/weights/carl_v0.1.pt", config, "weights/carl_v0.1.ccia", quantization=precision)

    elif cmd == "ayuda":
        print("Comandos: descargar [n], procesar [dir], entrenar [n], exportar [fp32/fp16], salir")

    else:
        print(f"Comando desconocido: {cmd}. Escribe 'ayuda' para ver opciones.")

def main():
    print("--- Bienvenido a la Consola de Carl IA ---")
    print("Escribe 'ayuda' para ver los comandos o 'salir' para terminar.")

    while True:
        try:
            # En modo interactivo para que no tenga que reiniciar el script
            user_input = input("Carl-CLI > ").strip()
            if user_input.lower() in ["salir", "exit", "quit"]:
                print("Saliendo de la consola de Carl. ¡Hasta pronto!")
                break
            if not user_input: continue
            run_command(user_input)
        except KeyboardInterrupt:
            print("\nSaliendo...")
            break
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    main()
