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
    net->global_dopamine = 0.5f;

    return net;
}

void cerebral_tick(CerebralNetwork *net) {
    if (!net) return;

    // 1. Update membrane potentials and check for spikes
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *n = &net->neurons[i];
        n->membrane_potential -= n->membrane_potential * LEAK_FACTOR * TIME_STEP;

        if (n->membrane_potential >= SPIKE_THRESHOLD) {
            n->has_spiked = true;
            n->last_spike_time = (float)net->current_time;
            n->membrane_potential = 0;
        } else {
            n->has_spiked = false;
        }
    }

    // 2. Propagate spikes and apply learning
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *pre = &net->neurons[i];

        // If PRE-synaptic neuron spikes
        if (pre->has_spiked) {
            for (uint32_t j = 0; j < pre->synapse_count; j++) {
                Synapse *s = &pre->synapses[j];
                if (s->target_neuron_id >= net->total_neurons) continue;
                Neuron *post = &net->neurons[s->target_neuron_id];

                // Propagate signal
                post->membrane_potential += s->weight;

                // LTD: Post fired BEFORE Pre.
                // If Post fired recently (in the last 10ms), weaken connection.
                float dt = (float)net->current_time - post->last_spike_time;
                if (dt > 0 && dt < 10.0f) {
                    s->weight -= 0.01f * (1.5f - net->global_dopamine);
                }
            }
        }

        // 3. Post-synaptic learning (LTP)
        // If POST-synaptic neuron spikes, check which PRE-synaptic neurons fired just before
        if (pre->has_spiked) {
            // We reuse 'pre' as 'post' here for the logic
            uint32_t post_id = i;
            for (uint32_t k = 0; k < net->total_neurons; k++) {
                Neuron *source = &net->neurons[k];
                for (uint32_t l = 0; l < source->synapse_count; l++) {
                    Synapse *s = &source->synapses[l];
                    if (s->target_neuron_id == post_id) {
                        // LTP: Source fired BEFORE Post.
                        float dt = (float)net->current_time - source->last_spike_time;
                        if (dt > 0 && dt < 10.0f) {
                            s->weight += 0.02f * net->global_dopamine;
                        }
                    }
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
