#include "cerebral.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

CerebralNetwork* cerebral_init(uint32_t neuron_count) {
    CerebralNetwork *net = (CerebralNetwork*)malloc(sizeof(CerebralNetwork));
    if (!net) return NULL;

    net->neurons = (Neuron*)calloc(neuron_count, sizeof(Neuron));
    if (!net->neurons) {
        free(net);
        return NULL;
    }

    net->total_neurons = neuron_count;
    net->current_time = 0.0;

    return net;
}

void cerebral_tick(CerebralNetwork *net) {
    if (!net) return;

    // First pass: Update membrane potentials and check for spikes
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *n = &net->neurons[i];

        // Leaky Integrate-and-Fire model
        n->membrane_potential -= n->membrane_potential * LEAK_FACTOR * TIME_STEP;

        if (n->membrane_potential >= SPIKE_THRESHOLD) {
            n->has_spiked = true;
            n->last_spike_time = (float)net->current_time;
            n->membrane_potential = 0; // Reset after spike
        } else {
            n->has_spiked = false;
        }
    }

    // Second pass: Propagate spikes through synapses
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *n = &net->neurons[i];
        if (n->has_spiked) {
            for (uint32_t j = 0; j < n->synapse_count; j++) {
                Synapse *s = &n->synapses[j];

                // Bounds check for target neuron
                if (s->target_neuron_id >= net->total_neurons) continue;

                Neuron *target = &net->neurons[s->target_neuron_id];

                // Add weighted spike to target membrane potential
                target->membrane_potential += s->weight;

                // STDP (Spike-Timing-Dependent Plasticity)
                float delta_t = target->last_spike_time - (float)net->current_time;
                if (delta_t > 0 && delta_t < 10.0f) {
                    s->weight += 0.01f; // Potentiation
                } else if (delta_t < 0 && delta_t > -10.0f) {
                    s->weight -= 0.005f; // Depression
                }
            }
        }
    }

    net->current_time += TIME_STEP;
}

void cerebral_stimulate(CerebralNetwork *net, uint32_t neuron_id, float current) {
    if (!net || neuron_id >= net->total_neurons) return;
    net->neurons[neuron_id].membrane_potential += current;
}

void cerebral_free(CerebralNetwork *net) {
    if (net) {
        if (net->neurons) free(net->neurons);
        free(net);
    }
}
