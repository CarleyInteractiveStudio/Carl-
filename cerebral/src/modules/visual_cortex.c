#include "visual_cortex.h"
#include <math.h>
#include <stdlib.h>

void visual_cortex_init(CerebralNetwork *net, VisualCortex *visual) {
    if (!net || !visual) return;

    visual->fovea_x = VISUAL_INPUT_SIZE / 2.0f;
    visual->fovea_y = VISUAL_INPUT_SIZE / 2.0f;

    // Map neurons 500-563 to the 8x8 foveal grid
    for (int i = 0; i < FOVEA_SIZE * FOVEA_SIZE; i++) {
        visual->visual_neurons[i] = 500 + i;
    }
}

void visual_cortex_process(CerebralNetwork *net, VisualCortex *visual, float *input_buffer) {
    if (!net || !visual || !input_buffer) return;

    int start_x = (int)visual->fovea_x - (FOVEA_SIZE / 2);
    int start_y = (int)visual->fovea_y - (FOVEA_SIZE / 2);

    for (int y = 0; y < FOVEA_SIZE; y++) {
        for (int x = 0; x < FOVEA_SIZE; x++) {
            int world_x = start_x + x;
            int world_y = start_y + y;

            if (world_x >= 0 && world_x < VISUAL_INPUT_SIZE && world_y >= 0 && world_y < VISUAL_INPUT_SIZE) {
                float pixel_val = input_buffer[world_y * VISUAL_INPUT_SIZE + world_x];
                // Stimulate corresponding visual neuron
                cerebral_stimulate(net, visual->visual_neurons[y * FOVEA_SIZE + x], pixel_val * 1.5f);
            }
        }
    }

    // Curiosity drive affects fovea movement (simulated behavior)
    if (net->curiosity_drive > 0.7f) {
        // Random "saccades" to find new info
        visual->fovea_x += (rand() % 5 - 2);
        visual->fovea_y += (rand() % 5 - 2);

        // Clamp to bounds
        if (visual->fovea_x < 4) visual->fovea_x = 4;
        if (visual->fovea_x > 60) visual->fovea_x = 60;
        if (visual->fovea_y < 4) visual->fovea_y = 4;
        if (visual->fovea_y > 60) visual->fovea_y = 60;
    }
}

void visual_cortex_move_fovea(VisualCortex *visual, float dx, float dy) {
    if (!visual) return;
    visual->fovea_x += dx;
    visual->fovea_y += dy;
}
