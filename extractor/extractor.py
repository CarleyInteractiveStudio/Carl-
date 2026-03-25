import os
import re
from scripts.registry import CarlRegistry

class CarlDataExtractor:
    """Extrae y limpia texto de diversos formatos eliminando ruido legal."""

    def __init__(self, output_dir="data/cleaned"):
        self.output_dir = output_dir
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        self.registry = CarlRegistry()

    def clean_text(self, text):
        # Normalizar espacios
        text = re.sub(r'\s+', ' ', text)
        return text.strip()

    def extract_from_txt(self, file_path):
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                text = f.read()

            # Buscar marcadores de Project Gutenberg
            start_markers = [
                "*** START OF THIS PROJECT GUTENBERG",
                "*** START OF THE PROJECT GUTENBERG",
                "START OF THIS PROJECT GUTENBERG",
                "START OF THE PROJECT GUTENBERG"
            ]
            end_markers = [
                "*** END OF THIS PROJECT GUTENBERG",
                "*** END OF THE PROJECT GUTENBERG",
                "END OF THIS PROJECT GUTENBERG",
                "END OF THE PROJECT GUTENBERG"
            ]

            text_upper = text.upper()

            # Corte inicial
            start_pos = -1
            for m in start_markers:
                pos = text_upper.find(m)
                if pos != -1:
                    start_pos = pos
                    # Saltar la línea del marcador
                    text = text[text.find("\n", start_pos)+1:]
                    text_upper = text.upper()
                    break

            # Corte final
            for m in end_markers:
                pos = text_upper.find(m)
                if pos != -1:
                    text = text[:text.rfind("\n", 0, pos)]
                    break

            # Limpieza línea a línea para eliminar residuos
            lines = text.splitlines()
            final_lines = []
            legal_keywords = ["PROJECT GUTENBERG", "LICENSE", "EBOOK", "COPYRIGHT"]

            for line in lines:
                up = line.upper()
                # Si la línea tiene demasiada carga legal, se descarta
                if sum(1 for kw in legal_keywords if kw in up) >= 2:
                    continue
                if "WWW.GUTENBERG.ORG" in up:
                    continue
                final_lines.append(line)

            # Si no se detectaron marcadores, aplicar recorte de seguridad
            if start_pos == -1 and len(final_lines) > 500:
                final_lines = final_lines[500:]

            return self.clean_text("\n".join(final_lines))
        except Exception as e:
            print(f"Error en {file_path}: {e}")
            return ""

    def process_directory(self, input_dir):
        files = [f for f in os.listdir(input_dir) if f.endswith('.txt')]
        all_text = ""
        for file in files:
            path = os.path.join(input_dir, file)
            if self.registry.is_processed(path): continue
            text = self.extract_from_txt(path)
            if text:
                all_text += text + "\n\n"
                self.registry.add_processed(path)

        with open(os.path.join(self.output_dir, "corpus_entrenamiento.txt"), "a", encoding="utf-8") as f:
            f.write(all_text)
        print("Procesamiento completado y limpio.")

if __name__ == "__main__":
    import sys
    ex = CarlDataExtractor()
    ex.process_directory(sys.argv[1] if len(sys.argv) > 1 else "data/input")
