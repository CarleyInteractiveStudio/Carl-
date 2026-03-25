import torch
import os
from torch.utils.data import Dataset, DataLoader
from tokenizer.tokenizer import CarlTokenizer
from model.model import CarlModel, CarlConfig
from tqdm import tqdm

class CarlDataset(Dataset):
    def __init__(self, text_file, tokenizer, block_size):
        with open(text_file, 'r', encoding='utf-8') as f:
            text = f.read()
        self.tokens = tokenizer.encode(text)
        print(f"Dataset: Total de tokens extraídos: {len(self.tokens)}")
        self.block_size = block_size

    def __len__(self):
        return len(self.tokens) - self.block_size

    def __getitem__(self, i):
        chunk = self.tokens[i:i + self.block_size + 1]
        x = torch.tensor(chunk[:-1], dtype=torch.long)
        y = torch.tensor(chunk[1:], dtype=torch.long)
        return x, y

def train_carl():
    # Configuración
    device = "cuda" if torch.cuda.is_available() else "cpu"
    block_size = 32
    batch_size = 4
    vocab_size = 5000
    learning_rate = 3e-4
    epochs = 1

    # 1. Cargar Tokenizador (o entrenar si no existe)
    tokenizer = CarlTokenizer(vocab_size=vocab_size)
    corpus_path = "data/cleaned/corpus_entrenamiento.txt"

    if not os.path.exists(corpus_path):
        # Crear un corpus mínimo de seguridad
        os.makedirs("data/cleaned", exist_ok=True)
        with open(corpus_path, "w") as f:
            f.write("Carl es una inteligencia artificial creada desde cero para aprender y ayudar.")

    with open(corpus_path, 'r', encoding='utf-8') as f:
        corpus_text = f.read()
    tokenizer.train(corpus_text)

    # 2. Preparar Datos (Limitamos el dataset para entrenamiento rapido en sandbox)
    print(f"Corpus text length: {len(corpus_text)}")
    dataset = CarlDataset(corpus_path, tokenizer, block_size)
    # Reducir mucho el vocabulario para la prueba
    vocab_size = 500
    tokenizer = CarlTokenizer(vocab_size=vocab_size)
    tokenizer.train(corpus_text)

    # Reducir para que se vea progreso en el sandbox
    if len(dataset) > 500:
        print("Dataset muy grande. Limitando a 500 ejemplos para demostración.")
        dataset.tokens = dataset.tokens[:500 + block_size]
    print(f"Dataset length: {len(dataset)}")
    loader = DataLoader(dataset, batch_size=batch_size, shuffle=True)

    # 3. Inicializar Modelo (Pequeño para demostración)
    config = CarlConfig(
        vocab_size=vocab_size,
        n_embd=128,
        n_head=4,
        n_layer=4,
        block_size=block_size
    )
    model = CarlModel(config).to(device)
    optimizer = torch.optim.AdamW(model.parameters(), lr=learning_rate)

    # 4. Bucle de Entrenamiento
    model.train()
    for epoch in range(epochs):
        pbar = tqdm(loader, desc=f"Epoch {epoch+1}")
        for x, y in pbar:
            x, y = x.to(device), y.to(device)
            logits, loss = model(x, y)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            pbar.set_postfix(loss=loss.item())

    # 5. Guardar modelo final en formato PyTorch (para luego exportar a .ccia)
    os.makedirs("model/weights", exist_ok=True)
    torch.save(model.state_dict(), "model/weights/carl_v0.1.pt")

    # 6. Calificar Entrenamiento (Mock de Loss Final)
    final_loss = loss.item() if 'loss' in locals() else 0.0
    score = max(0, 100 - (final_loss * 10))
    print(f"--- Calificación del Entrenamiento: {score:.1f}/100 ---")
    print(f"Entrenamiento completado. Pesos guardados en model/weights/carl_v0.1.pt")

if __name__ == "__main__":
    train_carl()
