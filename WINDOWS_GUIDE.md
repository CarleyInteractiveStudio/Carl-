# Guía de Entrenamiento de Carl IA en Windows (VS Code)

¡Felicidades! Estás a punto de entrenar a Carl con potencia real en tu PC. Sigue estos pasos:

### 1. Preparar el Entorno
1. Instala **Python 3.10+** (asegúrate de marcar "Add Python to PATH" en el instalador).
2. Abre la carpeta del proyecto en **Visual Studio Code**.
3. Abre una terminal en VS Code (`Ctrl + ñ` o `Terminal > New Terminal`).
4. Crea un entorno virtual (recomendado):
   ```powershell
   python -m venv venv
   .\venv\Scripts\activate
   ```

### 2. Instalar Dependencias
Ejecuta este comando para instalar todo lo necesario:
```powershell
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
pip install requests tqdm pypdf python-docx ebooklib beautifulsoup4 numpy
```
*(Nota: Si no tienes una tarjeta gráfica NVIDIA, usa `pip install torch` a secas).*

### 3. Descargar los Libros (Si aún no lo hiciste)
Para bajar los 200 libros en español:
```powershell
python scripts/book_downloader.py 200
```

### 4. Preparar el Corpus
Extrae el texto de todos los libros descargados:
```powershell
python extractor/extractor.py data/input
```

### 5. ¡INICIAR LA CONSOLA DE CARL!
He creado una consola interactiva para que no tengas que reiniciar el programa cada vez. Solo ejecútala una vez:
```powershell
$env:PYTHONPATH = "."
python carl_cli.py
```
Desde la consola podrás:
- **descargar 6000**: Descarga miles de libros.
- **procesar**: Extrae y limpia todo el texto.
- **entrenar 10**: Entrena a Carl por 10 épocas.
- **exportar fp16**: Exporta a formato binario comprimido para ahorrar RAM.

### Consejos para tu Hardware (16GB RAM / 2GB VRAM):
1. **Memoria (RAM):** Tienes 13GB libres, lo cual es **excelente** para procesar los 6,000 libros. El sistema cargará los textos en RAM sin problemas.
2. **Tarjeta Gráfica (VRAM):** Tienes 2GB integrados. He ajustado el `batch_size = 8` para que no se sature. Si ves un error de "Out of Memory", baja el `batch_size` a `4` en `trainer/train.py`.
3. **Velocidad:** Como es gráfica integrada, el entrenamiento puede tardar un poco. ¡Sé paciente con Carl, está aprendiendo mucho!

### 6. Exportar y Usar en C++
Una vez termine el entrenamiento, exporta el cerebro a formato `.ccia`:
```powershell
python scripts/exporter.py
```

Luego puedes compilar el motor C++ usando la extensión de C++ en VS Code y CMake para hablar con Carl de forma ultra rápida.

---
**¿Qué sigue?** Cuando Carl termine de entrenar, verás un archivo en `weights/carl_v0.1.ccia`. ¡Ese es el conocimiento puro de tu IA!
