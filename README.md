# Carl IA (Proyecto Carl)
## Modelo de Lenguaje de Propósito General (.ccia)

Carl es una IA construida desde cero, utilizando una arquitectura Transformer y un motor de inferencia propio escrito en C++.

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
