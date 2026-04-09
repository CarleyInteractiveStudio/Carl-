import os
import json
from collections import Counter
from tqdm import tqdm

class CarlTokenizer:
    """Implementación de un tokenizador BPE (Byte Pair Encoding) desde cero."""

    def __init__(self, vocab_size=10000):
        self.vocab_size = vocab_size
        self.vocab = {}
        self.merges = {}
        self.byte_encoder = self._bytes_to_unicode()
        self.byte_decoder = {v: k for k, v in self.byte_encoder.items()}

    def _bytes_to_unicode(self):
        """Mapea bytes a caracteres unicode legibles, similar a GPT-2."""
        bs = list(range(ord("!"), ord("~") + 1)) + list(range(ord("¡"), ord("¬") + 1)) + list(range(ord("®"), ord("ÿ") + 1))
        cs = bs[:]
        n = 0
        for b in range(2**8):
            if b not in bs:
                bs.append(b)
                cs.append(2**8 + n)
                n += 1
        cs = [chr(n) for n in cs]
        return dict(zip(bs, cs))

    def get_stats(self, ids):
        counts = Counter()
        for i in range(len(ids) - 1):
            counts[(ids[i], ids[i+1])] += 1
        return counts

    def merge(self, ids, pair, idx):
        """Optimiza el merge evitando recrear listas excesivamente."""
        new_ids = []
        i = 0
        while i < len(ids):
            if i < len(ids) - 1 and ids[i] == pair[0] and ids[i+1] == pair[1]:
                new_ids.append(idx)
                i += 2
            else:
                new_ids.append(ids[i])
                i += 1
        return new_ids

    def train(self, text, save_path="tokenizer/carl_vocab.json"):
        print("Entrenando Tokenizador de Carl...")
        # Inicializar con bytes individuales
        tokens = list(text.encode("utf-8"))

        # El vocabulario inicial son los 256 bytes
        self.vocab = {i: bytes([i]) for i in range(256)}
        self.merges = {} # Reset merges

        current_ids = list(tokens)
        num_merges = self.vocab_size - 256

        for i in tqdm(range(num_merges), desc="Merging tokens"):
            stats = self.get_stats(current_ids)
            if not stats or len(stats) == 0:
                break
            best_pair = max(stats, key=stats.get)
            if stats[best_pair] < 2: # Solo unir si aparece más de una vez
                break

            idx = 256 + i
            current_ids = self.merge(current_ids, best_pair, idx)
            self.merges[best_pair] = idx

            # Construir el nuevo token combinando los dos anteriores
            self.vocab[idx] = self.vocab[best_pair[0]] + self.vocab[best_pair[1]]

        self.save(save_path)

    def save(self, path):
        # Convertir bytes a lista de ints para JSON
        serializable_vocab = {k: list(v) for k, v in self.vocab.items()}
        # Convertir tuplas de merges a strings para JSON
        serializable_merges = {f"{k[0]},{k[1]}": v for k, v in self.merges.items()}

        data = {
            "vocab": serializable_vocab,
            "merges": serializable_merges
        }
        with open(path, "w", encoding="utf-8") as f:
            json.dump(data, f)
        print(f"Tokenizador guardado en: {path}")

    def load(self, path):
        with open(path, "r", encoding="utf-8") as f:
            data = json.load(f)

        self.vocab = {int(k): bytes(v) for k, v in data["vocab"].items()}
        self.merges = {tuple(map(int, k.split(","))): v for k, v in data["merges"].items()}

    def encode(self, text):
        """Convierte texto en una lista de IDs."""
        if not self.merges:
            return list(text.encode("utf-8"))
        tokens = list(text.encode("utf-8"))
        while len(tokens) >= 2:
            stats = self.get_stats(tokens)
            # Buscar qué pares de los presentes en el texto actual se pueden unir
            possible_merges = {p: self.merges[p] for p in stats if p in self.merges}
            if not possible_merges:
                break
            # El par a unir es el que tiene el índice de merge más pequeño
            pair = min(possible_merges.keys(), key=lambda p: self.merges[p])
            new_tokens = self.merge(tokens, pair, self.merges[pair])
            if len(new_tokens) == len(tokens): # Safety break
                break
            tokens = new_tokens
        return tokens

    def decode(self, ids):
        """Convierte una lista de IDs de vuelta a texto."""
        tokens = b"".join([self.vocab[idx] for idx in ids])
        return tokens.decode("utf-8", errors="replace")

if __name__ == "__main__":
    # Prueba rápida
    sample_text = "¡Hola Carl! Esta es una prueba del tokenizador BPE creado desde cero para el formato .ccia."
    tokenizer = CarlTokenizer(vocab_size=300)
    tokenizer.train(sample_text)

    encoded = tokenizer.encode("Hola Carl")
    print(f"Encoded: {encoded}")
    decoded = tokenizer.decode(encoded)
    print(f"Decoded: {decoded}")
