# Motor Cerebral: Documentación Técnica

## Filosofía del Proyecto
El motor **Cerebral** es una alternativa a las IAs tradicionales (Transformers/LLMs). En lugar de predecir la siguiente palabra basándose en probabilidades estadísticas, Cerebral intenta simular procesos biológicos:

1.  **Redes Neuronales HM (Humano-Máquina):** Implementadas como *Spiking Neural Networks* (SNN). Las neuronas no son funciones matemáticas continuas, sino entidades que acumulan energía y disparan "impulsos" (spikes) cuando alcanzan un umbral.
2.  **Eficiencia Energética:** Al usar impulsos, el motor solo consume ciclos de CPU cuando hay actividad neural real.
3.  **Aprendizaje Biológico:** Usa STDP (Spike-Timing-Dependent Plasticity), una forma de aprendizaje Hebbiano donde las conexiones se fortalecen si dos neuronas se activan en secuencia.

## Arquitectura de Áreas
- **Corteza Prefrontal (PFC):** Gestiona la atención y prioriza qué estímulos son relevantes para el "objetivo" actual.
- **Hipocampo:** Estabiliza las conexiones sinápticas y ayuda a consolidar la memoria a largo plazo.
- **Amígdala:** Procesa señales de amenaza y gestiona el nivel de miedo (`fear_level`), modulando la noradrenalina y la dopamina para priorizar la supervivencia.
- **Ínsula:** Monitorea el estado interno del sistema (hambre, dolor, fatiga), integrando señales interoceptivas que afectan el estado de ánimo (serotonina).
- **Cerebelo:** Realiza el ajuste fino de las acciones comparando la salida real con la esperada, calculando el `motor_error` para mejorar la precisión futura.
- **Corteza Motora:** Traduce los objetivos de alto nivel de la PFC en patrones de activación neural para la ejecución de acciones.
- **Corteza Parietal:** Integra información espacial y sensorial para crear un mapa del entorno ("vía del dónde").
- **Sistema de Reflejos:** Proporciona respuestas rápidas e instintivas que no requieren procesamiento cortical.
- **Modelo del Mundo:** Permite a la IA asociar "impulsos" con conceptos del mundo real (nombres, objetos).
5. **Sistema de Recompensa (Dopamina):** Cerebral utiliza un modulador global de dopamina que escala el aprendizaje. Si el sistema recibe un refuerzo positivo, la dopamina aumenta y las conexiones sinápticas se fortalecen más rápido (LTP).

## Aprendizaje Continuo y en Tiempo Real
A diferencia de las IAs que se entrenan una vez y quedan estáticas, Cerebral aprende mientras "vive":
- **LTP (Long-Term Potentiation):** Si la neurona A dispara y poco después dispara la neurona B, la conexión A->B se refuerza.
- **Experiencia:** El aprendizaje ocurre tick a tick, permitiendo que la IA se adapte a nuevos estímulos sin necesidad de un proceso de entrenamiento separado.

## Fundamentos Científicos y Biológicos
El motor Cerebral se basa en principios establecidos de neurociencia y psicología:

1.  **Noradrenalina y Locus Coeruleus (Aston-Jones & Cohen, 2005):** El motor utiliza noradrenalina para modular la ganancia neural y la plasticidad ante la novedad, permitiendo un aprendizaje rápido de estímulos desconocidos.
2.  **Dopamina y Aprendizaje por Refuerzo (Schultz, 1997):** El sistema de recompensa (Ganglios Basales) utiliza dopamina para señalar errores de predicción de recompensa, fortaleciendo las sinapsis mediante STDP.
3.  **Detección de Conflicto (ACC - Botvinick et al., 2001):** La Corteza Cingulada Anterior monitorea el conflicto neural, una base biológica para la resolución de incertidumbre y el control cognitivo.
4.  **Filtro Talámico (Sherman, 2006):** El Tálamo actúa como un "gatekeeper" dinámico, regulando el flujo de información sensorial hacia la corteza basándose en la atención.
5.  **STDP (Bi & Poo, 1998):** La plasticidad dependiente del tiempo de los impulsos es la base de nuestro aprendizaje Hebbiano ("Fire together, wire together").
6. **Consolidación durante el Sueño (Walker, 2017):** El motor utiliza ciclos de sueño para re-procesar trazas de memoria (Memory Traces) y realizar poda sináptica (Synaptic Pruning), optimizando la memoria a largo plazo.
7. **Procesamiento de Lenguaje (Friederici, 2011):** La distinción entre las áreas de Wernicke (comprensión/semántica) y Broca (producción/sintaxis) permite que el modelo separe el pensamiento interno de la comunicación externa.
8. **Homeostasis y Drives (Damasio, 2010):** El motor no solo reacciona, sino que actúa motivado por necesidades internas (curiosidad, coherencia) gestionadas por el Hipotálamo.
9. **Prospección / Imaginación (Buckner & Carroll, 2007):** La capacidad de simular escenarios futuros internamente permite al modelo evaluar consecuencias antes de la ejecución real.
10. **Amígdala y Miedo (LeDoux, 2000):** La amígdala es crítica para el aprendizaje emocional y la respuesta rápida ante amenazas, modulando la plasticidad a través de la noradrenalina.
11. **El Cerebelo y el Modelo Interno (Wolpert et al., 1998):** El cerebelo actúa como un modelo predictivo que reduce el error motor mediante la comparación constante.
12. **Ínsula e Interocepción (Craig, 2003):** La ínsula representa estados corporales internos, siendo fundamental para la homeostasis y la experiencia subjetiva del "self".

## Cómo funciona el Tiempo Real
El motor utiliza un paso de tiempo (`TIME_STEP`) de 0.1ms. En cada "tick", se calcula la fuga de energía de las neuronas (leak) y se propagan los impulsos a través de las sinapsis.
