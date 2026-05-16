#include "brain_modules.h"
#include <stdio.h>

void pfc_process(CerebralNetwork *net, uint32_t *active_goal_neurons, uint32_t count) {
    // In a real brain, the PFC provides top-down excitation to goal-relevant areas
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nid = active_goal_neurons[i];
        if (nid < net->total_neurons) {
            // Slightly boost the potential of neurons related to current "goals"
            net->neurons[nid].membrane_potential += 0.05f;
        }
    }
}

void reflex_trigger(CerebralNetwork *net, uint32_t input_id, uint32_t action_id) {
    // Reflexes are strong, direct connections
    if (net->neurons[input_id].has_spiked) {
        // Force a spike in the action neuron (instinct)
        net->neurons[action_id].membrane_potential = SPIKE_THRESHOLD + 0.1f;
    }
}

void hippocampus_consolidate(CerebralNetwork *net) {
    // Move short-term synaptic changes to long-term "stability"
    // For now, we simulate this by normalizing weights to prevent explosion/decay
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        for (uint32_t j = 0; j < net->neurons[i].synapse_count; j++) {
            Synapse *s = &net->neurons[i].synapses[j];
            if (s->weight > 5.0f) s->weight = 5.0f;
            if (s->weight < -1.0f) s->weight = -1.0f;
        }
    }
}
