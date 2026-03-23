import os
import re
import pypdf
from docx import Document
from ebooklib import epub
from bs4 import BeautifulSoup
from tqdm import tqdm

class CarlDataExtractor:
    """Extrae y limpia texto de diversos formatos para entrenar a Carl."""

    def __init__(self, output_dir="data/cleaned"):
        self.output_dir = output_dir
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)

    def clean_text(self, text):
        """Limpia el texto base eliminando espacios extra y caracteres no deseados."""
        # Normalizar espacios en blanco y saltos de línea excesivos
        text = re.sub(r'\s+', ' ', text)
        # Eliminar caracteres extraños pero mantener puntuación y acentos
        # text = re.sub(r'[^\w\s\.,;:\?!\(\)"\'áéíóúÁÉÍÓÚñÑ-]', '', text)
        return text.strip()

    def extract_from_pdf(self, file_path):
        text = ""
        try:
            with open(file_path, 'rb') as f:
                reader = pypdf.PdfReader(f)
                for page in reader.pages:
                    text += page.extract_text() + " "
        except Exception as e:
            print(f"Error procesando PDF {file_path}: {e}")
        return self.clean_text(text)

    def extract_from_docx(self, file_path):
        text = ""
        try:
            doc = Document(file_path)
            for para in doc.paragraphs:
                text += para.text + " "
        except Exception as e:
            print(f"Error procesando DOCX {file_path}: {e}")
        return self.clean_text(text)

    def extract_from_epub(self, file_path):
        text = ""
        try:
            book = epub.read_epub(file_path)
            for item in book.get_items_of_type(9): # 9 es el tipo HTML/Content
                soup = BeautifulSoup(item.get_body_content(), 'html.parser')
                text += soup.get_text() + " "
        except Exception as e:
            print(f"Error procesando EPUB {file_path}: {e}")
        return self.clean_text(text)

    def extract_from_html(self, file_path):
        text = ""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                soup = BeautifulSoup(f.read(), 'html.parser')
                text = soup.get_text()
        except Exception as e:
            print(f"Error procesando HTML {file_path}: {e}")
        return self.clean_text(text)

    def extract_from_txt(self, file_path):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                return self.clean_text(f.read())
        except Exception as e:
            print(f"Error procesando TXT {file_path}: {e}")
        return ""

    def process_directory(self, input_dir):
        """Procesa todos los archivos soportados en un directorio."""
        files = [f for f in os.listdir(input_dir) if os.path.isfile(os.path.join(input_dir, f))]
        all_text = ""

        for file in tqdm(files, desc="Extrayendo texto"):
            path = os.path.join(input_dir, file)
            ext = os.path.splitext(file)[1].lower()

            if ext == '.pdf':
                all_text += self.extract_from_pdf(path) + "\n\n"
            elif ext == '.docx':
                all_text += self.extract_from_docx(path) + "\n\n"
            elif ext == '.epub':
                all_text += self.extract_from_epub(path) + "\n\n"
            elif ext == '.html' or ext == '.htm':
                all_text += self.extract_from_html(path) + "\n\n"
            elif ext == '.txt':
                all_text += self.extract_from_txt(path) + "\n\n"

        output_file = os.path.join(self.output_dir, "corpus_entrenamiento.txt")
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(all_text)
        print(f"Extracción completada. Corpus guardado en: {output_file}")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Uso: python extractor/extractor.py <directorio_de_libros>")
    else:
        extractor = CarlDataExtractor()
        extractor.process_directory(sys.argv[1])
