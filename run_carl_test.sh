#!/bin/bash
set -e

echo "--- [1/5] Generando datos de prueba para Carl ---"
mkdir -p data/input
echo "Carl es una inteligencia artificial creada desde cero. Carl puede leer libros, ver imágenes y razonar en C++. Este es el inicio del gran proyecto Carl IA." > data/input/test_book.txt

echo "--- [2/5] Extrayendo texto con CarlDataExtractor ---"
python3 extractor/extractor.py data/input

echo "--- [3/5] Entrenando Micro-Carl (Pipeline de Texto) ---"
export PYTHONPATH=$PYTHONPATH:.
python3 trainer/train.py

echo "--- [4/5] Exportando cerebro a formato .ccia ---"
python3 scripts/exporter.py

echo "--- [5/5] Ejecutando Motor C++ con los pesos de Carl ---"
mkdir -p engine/build
cd engine/build
cmake .. > /dev/null
make > /dev/null
./carl_engine ../../weights/carl_v0.1.ccia

echo "--- ¡PRUEBA DE CARL COMPLETADA CON ÉXITO! ---"
