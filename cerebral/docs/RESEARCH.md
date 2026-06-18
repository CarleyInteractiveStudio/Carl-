# Brain Area Research for Cerebral Engine

## 1. Prefrontal Cortex (PFC)
- **Biological Role:** The "Executive Center". Responsible for decision-making, planning, and moderating social behavior. It holds "Working Memory".
- **Cerebral Implementation:** A high-level module that manages attention and goal-setting. It will act as a feedback loop for other modules, deciding which "thoughts" or "sensory inputs" should be prioritized.

## 2. Thalamus
- **Biological Role:** The "Relay Station". Every sensory input (except smell) passes through the Thalamus before reaching the cortex. It filters what is important.
- **Cerebral Implementation:** The input processing layer. It converts raw data (text, numbers) into spike patterns and applies an "attention mask" to reduce noise and compute consumption.

## 3. Hippocampus
- **Biological Role:** Memory consolidation. Converts short-term experiences into long-term memories and handles spatial navigation (creating a "map" of the environment).
- **Cerebral Implementation:** An associative memory system. It won't store data in a table but as "patterns" that can be reconstructed. It will handle the "World Model" by linking different sensory spikes into a unified concept.

## 4. Amygdala & Basal Ganglia
- **Biological Role:** Amygdala handles emotional response (fear/pleasure). Basal Ganglia handles motor control and habit formation via dopamine rewards.
- **Cerebral Implementation:** The "Instinct/Reflex" module. It provides a fast-path for responses that don't need "thinking". It also implements a Reward System to guide the learning of the entire network.

## 5. HM Neuron Model (Spiking Neural Network)
- **Model:** Leaky Integrate-and-Fire (LIF).
- **Learning:** Spike-Timing-Dependent Plasticity (STDP). This is the "Hebbian" principle: neurons that fire together, wire together.
- **Efficiency:** Since neurons only process when they "spike", the CPU remains idle for inactive parts of the network, drastically reducing consumption.
