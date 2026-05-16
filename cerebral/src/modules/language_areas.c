#include "language_areas.h"
#include <stdio.h>

void wernicke_process(CerebralNetwork *net, uint32_t *word_spike_sequence, uint32_t length) {
    if (!net || !word_spike_sequence) return;

    // Simula la comprensión semántica integrando impulsos secuenciales
    // En el cerebro, esto activa patrones en el lóbulo temporal superior
    for (uint32_t i = 0; i < length; i++) {
        cerebral_stimulate(net, word_spike_sequence[i], 0.8f);
    }
}

void broca_generate(CerebralNetwork *net, uint32_t concept_nid, uint32_t *output_sequence, uint32_t *out_length) {
    if (!net || !output_sequence) return;

    // Simula la planificación motora del habla
    // Broca transforma un "concepto" en una "secuencia" de acciones/palabras
    // Por ahora, simulamos una respuesta simple (Eco o asociación directa)
    output_sequence[0] = concept_nid; // El concepto mismo

    // Si la neurona del concepto tiene sinapsis fuertes, las añadimos a la secuencia
    uint32_t count = 1;
    Neuron *n = &net->neurons[concept_nid];
    for (uint32_t i = 0; i < n->synapse_count && count < 10; i++) {
        if (n->synapses[i].weight > 0.5f) {
            output_sequence[count++] = n->synapses[i].target_neuron_id;
        }
    }
    *out_length = count;
}
