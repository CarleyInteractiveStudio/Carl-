# Motor Cerebral: Documentación Técnica

## Filosofía del Proyecto
El motor **Cerebral** es una alternativa a las IAs tradicionales (Transformers/LLMs). En lugar de predecir la siguiente palabra basándose en probabilidades estadísticas, Cerebral intenta simular procesos biológicos:

1.  **Redes Neuronales HM (Humano-Máquina):** Implementadas como *Spiking Neural Networks* (SNN). Las neuronas no son funciones matemáticas continuas, sino entidades que acumulan energía y disparan "impulsos" (spikes) cuando alcanzan un umbral.
2.  **Eficiencia Energética:** Al usar impulsos, el motor solo consume ciclos de CPU cuando hay actividad neural real.
3.  **Aprendizaje Biológico:** Usa STDP (Spike-Timing-Dependent Plasticity), una forma de aprendizaje Hebbiano donde las conexiones se fortalecen si dos neuronas se activan en secuencia.

## Arquitectura de Áreas
- **Corteza Prefrontal (PFC):** Gestiona la atención y prioriza qué estímulos son relevantes para el "objetivo" actual.
- **Hipocampo:** Estabiliza las conexiones sinápticas y ayuda a consolidar la memoria a largo plazo.
- **Sistema de Reflejos:** Proporciona respuestas rápidas e instintivas que no requieren procesamiento cortical.
- **Modelo del Mundo:** Permite a la IA asociar "impulsos" con conceptos del mundo real (nombres, objetos).
5. **Sistema de Recompensa (Dopamina):** Cerebral utiliza un modulador global de dopamina que escala el aprendizaje. Si el sistema recibe un refuerzo positivo, la dopamina aumenta y las conexiones sinápticas se fortalecen más rápido (LTP).

## Aprendizaje Continuo y en Tiempo Real
A diferencia de las IAs que se entrenan una vez y quedan estáticas, Cerebral aprende mientras "vive":
- **LTP (Long-Term Potentiation):** Si la neurona A dispara y poco después dispara la neurona B, la conexión A->B se refuerza.
- **Experiencia:** El aprendizaje ocurre tick a tick, permitiendo que la IA se adapte a nuevos estímulos sin necesidad de un proceso de entrenamiento separado.

## Cómo funciona el Tiempo Real
El motor utiliza un paso de tiempo (`TIME_STEP`) de 0.1ms. En cada "tick", se calcula la fuga de energía de las neuronas (leak) y se propagan los impulsos a través de las sinapsis.
