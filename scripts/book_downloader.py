import requests
import os
import time
from tqdm import tqdm

class CarlBookDownloader:
    """Descarga libros en español de Project Gutenberg para entrenar a Carl."""

    def __init__(self, output_dir="data/input"):
        self.output_dir = output_dir
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        self.base_url = "https://www.gutenberg.org/cache/epub/"

    def download_spanish_books(self, count=200):
        print(f"Buscando {count} libros en español para Carl...")
        downloaded = 0
        # Project Gutenberg tiene IDs numéricos. Vamos a probar rangos comunes para español.
        # Algunos IDs conocidos de libros famosos en español:
        # 2000 (Don Quijote), 3500, etc.

        # Estrategia: Probar IDs y filtrar por idioma (usando la API de metadatos o probando descarga directa)
        # Para esta versión, usaremos una lista de IDs conocidos o probaremos secuencialmente

        current_id = 1000
        pbar = tqdm(total=count, desc="Descargando libros")

        while downloaded < count and current_id < 70000:
            try:
                # Intentar bajar el formato .txt (utf-8) que es mejor para Carl
                file_url = f"{self.base_url}{current_id}/pg{current_id}.txt"
                response = requests.get(file_url, timeout=5)

                if response.status_code == 200:
                    text = response.text
                    # Verificación simple de idioma: Buscar palabras comunes en español
                    spanish_keywords = [" el ", " la ", " que ", " por ", " con ", " para "]
                    if any(kw in text.lower() for kw in spanish_keywords):
                        file_path = os.path.join(self.output_dir, f"libro_{current_id}.txt")
                        with open(file_path, "w", encoding="utf-8") as f:
                            f.write(text)
                        downloaded += 1
                        pbar.update(1)
                        # Pequeña pausa para ser respetuosos con el servidor
                        time.sleep(0.5)

            except Exception:
                pass

            current_id += 1
            if current_id % 100 == 0:
                pbar.set_postfix(ID_actual=current_id)

        print(f"\nDescarga finalizada. {downloaded} libros listos en {self.output_dir}")

if __name__ == "__main__":
    downloader = CarlBookDownloader()
    # Para la prueba rápida bajaremos solo 5, pero el usuario pidió 200
    import sys
    count = 200
    if len(sys.argv) > 1:
        count = int(sys.argv[1])
    downloader.download_spanish_books(count)
