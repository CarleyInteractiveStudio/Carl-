#include "cerebral.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "language_areas.h"

CerebralNetwork* cerebral_init(uint32_t neuron_count) {
    CerebralNetwork *net = (CerebralNetwork*)malloc(sizeof(CerebralNetwork));
    if (!net) return NULL;

    net->neurons = (Neuron*)calloc(neuron_count, sizeof(Neuron));
    if (!net->neurons) {
        free(net);
        return NULL;
    }

    net->traces = (MemoryTrace*)calloc(MAX_MEMORY_TRACES, sizeof(MemoryTrace));
    if (!net->traces) {
        free(net->neurons);
        free(net);
        return NULL;
    }

    net->total_neurons = neuron_count;
    net->current_time = 0.0;
    net->global_dopamine = 0.5f;
    net->global_noradrenaline = 0.5f;
    net->curiosity_drive = 0.5f;
    net->coherence_drive = 0.5f;

    net->hunger_level = 0.0f;
    net->pain_level = 0.0f;
    net->fatigue_level = 0.0f;
    net->fear_level = 0.0f;
    net->motor_error = 0.0f;
    net->global_serotonin = 0.5f;

    net->is_simulating = false;

    net->visual_input = (float*)calloc(64*64, sizeof(float));
    net->auditory_input = (float*)calloc(32, sizeof(float));

    return net;
}

void cerebral_tick(CerebralNetwork *net) {
    if (!net) return;

    // 1. Update membrane potentials and check for spikes
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *n = &net->neurons[i];

        // Noradrenaline reduces the "leak", making neurons more excitable (alert)
        float current_leak = LEAK_FACTOR * (1.1f - net->global_noradrenaline);
        n->membrane_potential -= n->membrane_potential * current_leak * TIME_STEP;

        if (n->membrane_potential >= SPIKE_THRESHOLD) {
            n->has_spiked = true;
            n->last_spike_time = (float)net->current_time;
            n->membrane_potential = 0;

            // Record memory trace
            if (net->trace_count < MAX_MEMORY_TRACES) {
                net->traces[net->trace_count].neuron_id = i;
                net->traces[net->trace_count].timestamp = (float)net->current_time;
                net->trace_count++;
            }
        } else {
            n->has_spiked = false;
        }
    }

    // 1.5 Background activity (Default Mode Network simulation)
    // Spontaneous firing of random neurons to simulate autonomous thought
    if ((uint32_t)(net->current_time / TIME_STEP) % 100 == 0) {
        uint32_t random_neuron = rand() % net->total_neurons;
        net->neurons[random_neuron].membrane_potential += 0.5f;
    }

    // 2. Propagate spikes and apply learning
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *pre = &net->neurons[i];

        // 2.1 Inner Monologue Loop (Broca -> Wernicke)
        // If a neuron in the "Broca" range spikes, it feeds back into "Wernicke"
        // This is a simplified simulation of the inner voice loop.
        if (pre->has_spiked && i >= 800 && i < 900) { // Assuming 800-900 is Broca
             uint32_t wernicke_target = i - 200; // Assuming 600-700 is Wernicke
             cerebral_stimulate(net, wernicke_target, 0.5f);
        }

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
                            // In simulation mode, learning is temporary or reduced
                            float scale = net->is_simulating ? 0.1f : 1.0f;
                            // Noradrenaline also accelerates synaptic plastic changes
                            float learning_rate = 0.02f * net->global_dopamine * (0.5f + net->global_noradrenaline) * scale;
                            s->weight += learning_rate;
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

void cerebral_inflict_pain(CerebralNetwork *net, float intensity) {
    if (!net) return;
    // Pain increases pain_level in Insula and drops dopamine
    net->pain_level += intensity;
    if (net->pain_level > 1.0f) net->pain_level = 1.0f;

    net->global_dopamine -= intensity * 0.5f;
    if (net->global_dopamine < 0.0f) net->global_dopamine = 0.0f;

    // High pain increases noradrenaline (stress response)
    net->global_noradrenaline += intensity * 0.2f;
    if (net->global_noradrenaline > 1.0f) net->global_noradrenaline = 1.0f;
}

void cerebral_free(CerebralNetwork *net) {
    if (net) {
        if (net->neurons) free(net->neurons);
        if (net->traces) free(net->traces);
        if (net->visual_input) free(net->visual_input);
        if (net->auditory_input) free(net->auditory_input);
        free(net);
    }
}
