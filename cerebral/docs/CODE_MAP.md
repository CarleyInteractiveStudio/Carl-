# Mapa de Código - Motor Cerebral

Este documento detalla la ubicación de cada componente lógico del motor.

## 1. Núcleo Neural (HM Engine)
- **Archivo:** `cerebral/src/core/hm_engine.c`
  - `cerebral_init` (Línea 6): Inicializa la red y reserva memoria.
  - `cerebral_tick` (Línea 22): El ciclo principal de pensamiento.
  - `cerebral_stimulate` (Línea 65): Inyecta energía externa a una neurona (sentidos).
- **Archivo:** `cerebral/include/cerebral.h`
  - Definición de `Neuron` y `Synapse`: Estructuras fundamentales.

## 2. Áreas Cerebrales (Brain Modules)
- **Archivo:** `cerebral/src/modules/brain_modules.c`
  - `pfc_process` (Línea 4): Lógica de atención de la Corteza Prefrontal.
  - `reflex_trigger` (Línea 14): Sistema de instintos/reflejos.
  - `hippocampus_consolidate` (Línea 21): Consolidación de memoria.

## 3. Modelo del Mundo y Senses
- **Archivo:** `cerebral/src/modules/world_model.c`
  - `world_model_add_concept`: Mapea nombres a neuronas.
  - `world_model_perceive`: Convierte texto en impulsos eléctricos.

## 4. Pruebas y Verificación
- **Archivo:** `cerebral/tests/test_core.c`
  - `test_neuron_spike`: Verifica que la propagación de impulsos funcione correctamente.
