#ifndef AUDITORY_CORTEX_H
#define AUDITORY_CORTEX_H

#include "cerebral.h"

#define FREQUENCY_BANDS 32

typedef struct {
    uint32_t neuron_ids[FREQUENCY_BANDS];
    float frequency_activations[FREQUENCY_BANDS];
} AuditoryCortex;

// Initializes the auditory cortex mapping neurons to frequency bands
void auditory_cortex_init(CerebralNetwork *net, AuditoryCortex *auditory);

// Processes an array of frequency magnitudes (from FFT) and stimulates neurons
void auditory_cortex_process(CerebralNetwork *net, AuditoryCortex *auditory, float *magnitudes);

#endif
