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
  - `basal_ganglia_update` (Línea 15): Gestión de recompensas y dopamina.
  - `locus_coeruleus_update` (Línea 27): Noradrenalina y alerta ante novedad.
  - `acc_detect_conflict` (Línea 41): Detección de conflicto neural e incertidumbre.
  - `association_cortex_connect` (Línea 59): Creación de conexiones espontáneas.
  - `thalamus_gate_input` (Línea 71): Filtrado sensorial por atención.
  - `cerebral_sleep` (Línea 77): Ciclo de sueño, re-activación y poda.
  - `hypothalamus_update` (Línea 106): Gestión de necesidades biológicas (Drives).
  - `pfc_simulate_prospect` (Línea 129): Imaginación y simulación mental.
  - `reflex_trigger` (Línea 151): Sistema de instintos/reflejos.
  - `hippocampus_consolidate` (Línea 158): Consolidación de memoria.
  - `amygdala_process`: Procesamiento de miedo y amenazas.
  - `insula_monitor`: Monitoreo de estados internos (hambre, dolor).
  - `cerebellum_coordinate`: Ajuste motor y detección de error.
  - `motor_cortex_plan`: Planificación de acciones.
  - `parietal_spatial_map`: Procesamiento de coordenadas espaciales.
  - `cerebral_inflict_pain` (hm_engine.c): Inyección de señales de castigo.

## 3. Exportador de Pensamientos (.ccp)
- **Archivo:** `cerebral/src/modules/ccp_exporter.c`
  - `ccp_exporter_record_frame`: Guarda el estado mental actual.

## 4. Lenguaje (Language Areas)
- **Archivo:** `cerebral/src/modules/language_areas.c`
  - `wernicke_process`: Comprensión semántica.
  - `broca_generate`: Planificación y producción del habla.

## 5. Sentidos y Actuación (Sensory-Motor)
- **Archivo:** `cerebral/src/modules/auditory_cortex.c`: Cóclea digital y procesamiento de frecuencia.
- **Archivo:** `cerebral/src/modules/visual_cortex.c`: Fóvea atencional y barrido visual.
- **Archivo:** `cerebral/src/modules/vocal_tract.c`: Síntesis de voz en tiempo real.

## 6. Modelo del Mundo y Senses
- **Archivo:** `cerebral/src/modules/world_model.c`
  - `world_model_add_concept`: Mapea nombres a neuronas.
  - `world_model_perceive`: Convierte texto en impulsos eléctricos.

## 4. Pruebas y Verificación
- **Archivo:** `cerebral/src/main.c`: Ejemplo principal de aprendizaje en tiempo real.
- **Archivo:** `cerebral/tests/test_core.c`
  - `test_neuron_spike`: Verifica que la propagación de impulsos funcione correctamente.
