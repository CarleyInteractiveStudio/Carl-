#ifndef VOCAL_TRACT_H
#define VOCAL_TRACT_H

#include "cerebral.h"

#define SAMPLE_RATE 44100
#define CHANNELS 1

typedef struct {
    float frequency;
    float amplitude;
    float phase;
    uint32_t motor_neuron_start;
} VocalTract;

void vocal_tract_init(VocalTract *vocal, uint32_t motor_neuron_start);

// Generates the next audio sample based on motor neuron activations
float vocal_tract_generate_sample(CerebralNetwork *net, VocalTract *vocal);

#endif
