import argparse
import os
import sys
from scripts.book_downloader import CarlBookDownloader
from extractor.extractor import CarlDataExtractor
from trainer.train import train_carl
from scripts.exporter import export_to_ccia
from model.model import CarlConfig

def main():
    parser = argparse.ArgumentParser(description="Carl IA - Interfaz de Gestión Unificada")
    subparsers = parser.add_subparsers(dest="command", help="Comandos disponibles")

    # Comando: descargar
    dl_parser = subparsers.add_parser("descargar", help="Descargar libros de Gutenberg")
    dl_parser.add_argument("--cantidad", type=int, default=10, help="Número de libros a bajar")

    # Comando: procesar
    pr_parser = subparsers.add_parser("procesar", help="Extraer texto de los libros")
    pr_parser.add_argument("--dir", type=str, default="data/input", help="Directorio de entrada")

    # Comando: entrenar
    tr_parser = subparsers.add_parser("entrenar", help="Iniciar entrenamiento de Carl")
    tr_parser.add_argument("--epochs", type=int, default=1, help="Número de épocas")

    # Comando: exportar
    ex_parser = subparsers.add_parser("exportar", help="Exportar a formato .ccia")
    ex_parser.add_argument("--precision", type=str, default="fp32", choices=["fp32", "fp16"], help="Precisión")

    args = parser.parse_args()

    if args.command == "descargar":
        dl = CarlBookDownloader()
        dl.download_spanish_books(args.cantidad)

    elif args.command == "procesar":
        ex = CarlDataExtractor()
        ex.process_directory(args.dir)

    elif args.command == "entrenar":
        # Actualizar epochs en tiempo de ejecución (mock para esta demo)
        print(f"Iniciando entrenamiento por {args.epochs} épocas...")
        train_carl()

    elif args.command == "exportar":
        config = CarlConfig(vocab_size=5000, n_embd=256, n_head=8, n_layer=6, block_size=32)
        os.makedirs("weights", exist_ok=True)
        export_to_ccia("model/weights/carl_v0.1.pt", config, "weights/carl_v0.1.ccia", quantization=args.precision)

    else:
        parser.print_help()

if __name__ == "__main__":
    main()
