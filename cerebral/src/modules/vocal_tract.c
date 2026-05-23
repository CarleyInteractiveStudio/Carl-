#include "vocal_tract.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void vocal_tract_init(VocalTract *vocal, uint32_t motor_neuron_start) {
    if (!vocal) return;
    vocal->frequency = 0.0f;
    vocal->amplitude = 0.0f;
    vocal->phase = 0.0f;
    vocal->motor_neuron_start = motor_neuron_start;
}

float vocal_tract_generate_sample(CerebralNetwork *net, VocalTract *vocal) {
    if (!net || !vocal) return 0.0f;

    // Use motor neurons (e.g., 900-910) to control vocal parameters
    // Neuron 900: Controls base frequency (pitch)
    // Neuron 901: Controls amplitude (volume)

    float target_freq = 0.0f;
    float target_amp = 0.0f;

    if (net->neurons[vocal->motor_neuron_start].membrane_potential > 0.5f) {
        target_freq = 110.0f + net->neurons[vocal->motor_neuron_start].membrane_potential * 100.0f;
    }

    if (net->neurons[vocal->motor_neuron_start + 1].membrane_potential > 0.5f) {
        target_amp = net->neurons[vocal->motor_neuron_start + 1].membrane_potential * 0.5f;
    }

    // Smooth transitions
    vocal->frequency = 0.99f * vocal->frequency + 0.01f * target_freq;
    vocal->amplitude = 0.99f * vocal->amplitude + 0.01f * target_amp;

    if (vocal->amplitude < 0.001f) return 0.0f;

    // Generate sine wave (simplified voice simulation)
    float sample = vocal->amplitude * sinf(vocal->phase);

    vocal->phase += 2.0f * M_PI * vocal->frequency / SAMPLE_RATE;
    if (vocal->phase > 2.0f * M_PI) vocal->phase -= 2.0f * M_PI;

    return sample;
}
