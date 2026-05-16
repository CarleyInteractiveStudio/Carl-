#ifndef CEREBRAL_H
#define CEREBRAL_H

#include <stdint.h>
#include <stdbool.h>

/* Global configuration for the Cerebral Engine */
#define MAX_NEURONS 1000
#define MAX_SYNAPSES_PER_NEURON 100
#define TIME_STEP 0.1f // ms
#define SPIKE_THRESHOLD 1.0f
#define LEAK_FACTOR 0.1f

typedef struct {
    float weight;
    uint32_t target_neuron_id;
    float last_spike_time; // For STDP learning
} Synapse;

typedef struct {
    float membrane_potential;
    float last_spike_time;
    Synapse synapses[MAX_SYNAPSES_PER_NEURON];
    uint32_t synapse_count;
    bool has_spiked;
} Neuron;

typedef struct {
    Neuron *neurons;
    uint32_t total_neurons;
    double current_time;
    float global_dopamine;     // Reward/Motivation (LTP boost)
    float global_noradrenaline; // Alertness/Novelty (Excitability/Learning speed)
} CerebralNetwork;

// Core functions
CerebralNetwork* cerebral_init(uint32_t neuron_count);
void cerebral_tick(CerebralNetwork *net);
void cerebral_stimulate(CerebralNetwork *net, uint32_t neuron_id, float current);
void cerebral_free(CerebralNetwork *net);

#endif // CEREBRAL_H
