# Guía de Entrenamiento de Carl IA en Kaggle

Kaggle te da acceso gratuito a GPUs potentes. Sigue estos pasos para entrenar a Carl:

### 1. Crear un Notebook
1. Ve a [kaggle.com](https://www.kaggle.com) y crea un nuevo Notebook.
2. En el menú de la derecha (**Settings**), activa la **GPU** (P100 o T4) y el **Internet**.

### 2. Subir el Código de Carl
Puedes subir los archivos directamente o clonarlos. En la primera celda, instala las dependencias:
```python
!pip install requests tqdm pypdf python-docx ebooklib beautifulsoup4 numpy torch
```

### 3. Usar el Cerebro Neuronal (C++ / Velocidad Extra)
Kaggle usa Linux. Para compilar las conexiones neuronales de Carl allí:
```python
!mkdir -p engine/build
!cd engine/build && cmake .. && make
```

### 4. Ejecutar el Entrenamiento
Usa la consola de Carl directamente desde el Notebook:
```python
import os
os.environ['PYTHONPATH'] = "."
!python carl_cli.py <<EOF
descargar 6000
procesar
entrenar 10
exportar fp16
salir
EOF
```

### 5. Descargar los Pesos
Al finalizar, encontrarás el archivo `weights/carl_v0.1.ccia` en la carpeta `/kaggle/working/`. ¡Descárgalo y llévatelo a tu PC!

---
**Tip Pro:** Si quieres que Carl sea aún más rápido, usa el comando `entrenar_cpp` dentro de la consola para saltarte gran parte de la sobrecarga de Python.
