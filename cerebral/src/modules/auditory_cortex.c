#include "auditory_cortex.h"
#include <math.h>

void auditory_cortex_init(CerebralNetwork *net, AuditoryCortex *auditory) {
    if (!net || !auditory) return;

    // We map a range of neurons to represent different frequencies (tonotopy)
    // Assuming neurons 400-431 are reserved for primary auditory cortex (A1)
    for (int i = 0; i < FREQUENCY_BANDS; i++) {
        auditory->neuron_ids[i] = 400 + i;
        auditory->frequency_activations[i] = 0.0f;
    }
}

void auditory_cortex_process(CerebralNetwork *net, AuditoryCortex *auditory, float *magnitudes) {
    if (!net || !auditory || !magnitudes) return;

    for (int i = 0; i < FREQUENCY_BANDS; i++) {
        float magnitude = magnitudes[i];

        // Tonotopic stimulation: Each band stimulates its corresponding neuron
        // Higher magnitude -> stronger stimulation
        if (magnitude > 0.01f) {
            cerebral_stimulate(net, auditory->neuron_ids[i], magnitude * 2.0f);
        }

        // Decay activation for monitoring
        auditory->frequency_activations[i] = magnitude;
    }

    // Auditory input increases alertness (noradrenaline) if loud/sudden
    float sum = 0;
    for(int i=0; i<FREQUENCY_BANDS; i++) sum += magnitudes[i];
    if (sum > 5.0f) {
        net->global_noradrenaline += 0.02f;
        if (net->global_noradrenaline > 1.0f) net->global_noradrenaline = 1.0f;
    }
}
