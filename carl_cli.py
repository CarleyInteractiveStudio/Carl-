import os
import sys
from scripts.book_downloader import CarlBookDownloader
from extractor.extractor import CarlDataExtractor
from trainer.train import train_carl
from scripts.exporter import export_to_ccia
from scripts.chat import run_carl_chat
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
        # Uso: procesar [cantidad] o procesar (todos)
        limit = int(parts[1]) if len(parts) > 1 else None
        ex = CarlDataExtractor()
        ex.process_directory("data/input", limit=limit)

    elif cmd == "entrenar":
        epochs = int(parts[1]) if len(parts) > 1 else 1
        print(f"Iniciando entrenamiento por {epochs} épocas...")
        train_carl()

    elif cmd == "entrenar_cpp":
        print("Compilando y ejecutando entrenador de alto rendimiento (C++)...")
        os.system("mkdir -p engine/build && cd engine/build && cmake .. && make && ./carl_trainer")

    elif cmd == "exportar":
        precision = parts[1] if len(parts) > 1 else "fp32"
        config = CarlConfig(vocab_size=10000, n_embd=256, n_head=8, n_layer=6, block_size=64)
        os.makedirs("weights", exist_ok=True)
        export_to_ccia("model/weights/carl_v0.1.pt", config, "weights/carl_v0.1.ccia", quantization=precision)

    elif cmd == "chat":
        print("--- Entrando a modo Chat con Carl ---")
        run_carl_chat()

    elif cmd == "lista":
        from scripts.registry import CarlRegistry
        reg = CarlRegistry()
        print(f"--- Estadísticas de Carl ---")
        print(f"Libros descargados: {len(reg.data['downloaded_ids'])}")
        print(f"Libros procesados: {len(reg.data['processed_files'])}")

    elif cmd == "limpiar_corpus":
        path = "data/cleaned/corpus_entrenamiento.txt"
        if os.path.exists(path):
            os.remove(path)
            print("Corpus de entrenamiento eliminado. Listo para una nueva extracción.")

    elif cmd == "ayuda":
        print("Comandos: descargar [n], procesar [n], entrenar [n], entrenar_cpp, exportar [fp32/fp16], chat, lista, limpiar_corpus, salir")

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
