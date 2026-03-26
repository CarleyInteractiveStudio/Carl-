import torch
import torch.nn as nn
import torch.nn.functional as F
import math
from model.native_ops import carl_matmul

class CarlConfig:
    def __init__(self, vocab_size=10000, n_embd=256, n_head=8, n_layer=12, block_size=256):
        self.vocab_size = vocab_size
        self.n_embd = n_embd
        self.n_head = n_head
        self.n_layer = n_layer
        self.block_size = block_size

class MultiHeadAttention(nn.Module):
    def __init__(self, config):
        super().__init__()
        assert config.n_embd % config.n_head == 0
        self.n_head = config.n_head
        self.key = nn.Linear(config.n_embd, config.n_embd)
        self.query = nn.Linear(config.n_embd, config.n_embd)
        self.value = nn.Linear(config.n_embd, config.n_embd)
        self.proj = nn.Linear(config.n_embd, config.n_embd)
        self.register_buffer("mask", torch.tril(torch.ones(config.block_size, config.block_size))
                                     .view(1, 1, config.block_size, config.block_size))

    def forward(self, x):
        B, T, C = x.size()
        k = self.key(x).view(B, T, self.n_head, C // self.n_head).transpose(1, 2)
        q = self.query(x).view(B, T, self.n_head, C // self.n_head).transpose(1, 2)
        v = self.value(x).view(B, T, self.n_head, C // self.n_head).transpose(1, 2)

        # Usamos los músculos de C++ para la multiplicación de atención
        # reshaped para 2D si es necesario o directo si el autograd lo permite
        att = carl_matmul(q.reshape(-1, C // self.n_head), k.transpose(-2, -1).reshape(C // self.n_head, -1))
        att = att.reshape(B, self.n_head, T, T)
        att = att * (1.0 / math.sqrt(k.size(-1)))

        att = att.masked_fill(self.mask[:, :, :T, :T] == 0, float('-inf'))
        att = F.softmax(att, dim=-1)

        y = carl_matmul(att.reshape(-1, T), v.reshape(T, -1))
        y = y.reshape(B, self.n_head, T, C // self.n_head)
        y = y.transpose(1, 2).contiguous().view(B, T, C)
        return self.proj(y)

class FeedForward(nn.Module):
    def __init__(self, config):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(config.n_embd, 4 * config.n_embd),
            nn.GELU(),
            nn.Linear(4 * config.n_embd, config.n_embd),
        )

    def forward(self, x):
        return self.net(x)

class Block(nn.Module):
    def __init__(self, config):
        super().__init__()
        self.ln1 = nn.LayerNorm(config.n_embd)
        self.attn = MultiHeadAttention(config)
        self.ln2 = nn.LayerNorm(config.n_embd)
        self.ff = FeedForward(config)

    def forward(self, x):
        x = x + self.attn(self.ln1(x))
        x = x + self.ff(self.ln2(x))
        return x

class CarlModel(nn.Module):
    """Arquitectura de Carl basada en Transformer Decoder con soporte Multimodal."""
    def __init__(self, config):
        super().__init__()
        self.config = config
        self.transformer = nn.ModuleDict(dict(
            wte = nn.Embedding(config.vocab_size, config.n_embd),
            wpe = nn.Embedding(config.block_size, config.n_embd),
            # Puente de visión: Convierte parches de imagen 16x16 (3 canales) en el espacio de Carl
            vision_proj = nn.Linear(16 * 16 * 3, config.n_embd),
            h = nn.ModuleList([Block(config) for _ in range(config.n_layer)]),
            ln_f = nn.LayerNorm(config.n_embd),
        ))
        self.lm_head = nn.Linear(config.n_embd, config.vocab_size, bias=False)

    def forward(self, idx, targets=None, image_patches=None):
        device = idx.device
        b, t = idx.size()
        pos = torch.arange(0, t, dtype=torch.long, device=device).unsqueeze(0)

        tok_emb = self.transformer.wte(idx)

        if image_patches is not None:
            # Si hay imagen, proyectamos los parches y los concatenamos al principio
            img_emb = self.transformer.vision_proj(image_patches)
            tok_emb = torch.cat((img_emb, tok_emb), dim=1)
            t = tok_emb.size(1)
            pos = torch.arange(0, t, dtype=torch.long, device=device).unsqueeze(0)

        pos_emb = self.transformer.wpe(pos[:, :tok_emb.size(1)])
        x = tok_emb + pos_emb

        for block in self.transformer.h:
            x = block(x)

        x = self.transformer.ln_f(x)
        logits = self.lm_head(x)

        loss = None
        if targets is not None:
            loss = F.cross_entropy(logits.view(-1, logits.size(-1)), targets.view(-1))

        return logits, loss

    def generate(self, idx, max_new_tokens):
        for _ in range(max_new_tokens):
            idx_cond = idx[:, -self.config.block_size:]
            logits, _ = self(idx_cond)
            logits = logits[:, -1, :]
            probs = F.softmax(logits, dim=-1)
            idx_next = torch.multinomial(probs, num_samples=1)
            idx = torch.cat((idx, idx_next), dim=1)
        return idx

if __name__ == "__main__":
    # Prueba del modelo
    config = CarlConfig(vocab_size=1000, n_embd=64, n_head=4, n_layer=4, block_size=32)
    model = CarlModel(config)

    # Parámetros aproximados
    params = sum(p.numel() for p in model.parameters())
    print(f"Modelo Carl (Proto) inicializado con {params:,} parámetros.")

    dummy_input = torch.randint(0, 1000, (1, 32))
    logits, _ = model(dummy_input)
    print(f"Output shape: {logits.shape}")
