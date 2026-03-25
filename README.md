# Carl IA (Proyecto Carl)
## Modelo de Lenguaje de Propósito General (.ccia)

Carl es una Inteligencia Artificial construida **100% desde cero**.

### Originalidad de Carl
- **Sin Modelos Base:** Carl NO utiliza GPT-2, Llama, Mistral ni ningún otro modelo pre-entrenado. El cerebro ha sido programado neurona por neurona en este repositorio.
- **Arquitectura Propia:** Implementación manual de capas Transformer Multimodales en PyTorch.
- **Vocabulario Único:** Tokenizador BPE programado desde cero que entrena un vocabulario basado exclusivamente en tus libros.
- **Motor C++ Independiente:** Motor de inferencia escrito en C++17 sin dependencias de frameworks de terceros.

### Estructura del Proyecto
- `extractor/`: Herramientas para extraer texto de PDF, EPUB, DOCX, etc.
- `tokenizer/`: Implementación del Tokenizador BPE propio.
- `model/`: Definición de la arquitectura Transformer de Carl (PyTorch).
- `trainer/`: Pipeline de entrenamiento y preparación de datos.
- `engine/`: Motor de inferencia en C++ para archivos `.ccia`.
- `scripts/`: Utilidades para exportar y convertir modelos.
- `data/`: Directorio para datos de entrenamiento.

### Requisitos
- Python 3.8+
- PyTorch
- Eigen (para el motor C++)
- CMake 3.10+
- G++ (C++17)
