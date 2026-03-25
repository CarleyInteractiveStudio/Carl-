import json
import os

class CarlRegistry:
    """Sistema de registro para evitar duplicados en descargas y extracciones de Carl."""

    def __init__(self, registry_file="data/carl_registry.json"):
        self.registry_file = registry_file
        self.data = self._load()

    def _load(self):
        default = {"downloaded_ids": [], "processed_files": []}
        if os.path.exists(self.registry_file):
            try:
                with open(self.registry_file, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                    # Asegurar que las llaves existan
                    for key in default:
                        if key not in data: data[key] = []
                    return data
            except:
                return default
        return default

    def save(self):
        os.makedirs(os.path.dirname(self.registry_file), exist_ok=True)
        with open(self.registry_file, 'w', encoding='utf-8') as f:
            json.dump(self.data, f, indent=4)

    def is_downloaded(self, book_id):
        return str(book_id) in self.data["downloaded_ids"]

    def add_downloaded(self, book_id):
        if str(book_id) not in self.data["downloaded_ids"]:
            self.data["downloaded_ids"].append(str(book_id))
            self.save()

    def is_processed(self, file_path):
        return file_path in self.data["processed_files"]

    def add_processed(self, file_path):
        if file_path not in self.data["processed_files"]:
            self.data["processed_files"].append(file_path)
            self.save()
