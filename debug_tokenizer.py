from tokenizer.tokenizer import CarlTokenizer

t = CarlTokenizer(vocab_size=300)
text = "Carl es una inteligencia artificial creada desde cero para aprender y ayudar."
t.train(text)
print(f"Vocab size: {len(t.vocab)}")
print(f"Merges: {t.merges}")
encoded = t.encode(text)
print(f"Encoded length: {len(encoded)}")
print(f"Encoded: {encoded}")
decoded = t.decode(encoded)
print(f"Decoded: {decoded}")
