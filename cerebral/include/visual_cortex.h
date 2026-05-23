#ifndef VISUAL_CORTEX_H
#define VISUAL_CORTEX_H

#include "cerebral.h"

#define FOVEA_SIZE 8 // 8x8 grid of focus
#define VISUAL_INPUT_SIZE 64 // 64x64 total field

typedef struct {
    float fovea_x;
    float fovea_y;
    uint32_t visual_neurons[FOVEA_SIZE * FOVEA_SIZE];
} VisualCortex;

void visual_cortex_init(CerebralNetwork *net, VisualCortex *visual);
void visual_cortex_process(CerebralNetwork *net, VisualCortex *visual, float *input_buffer);
void visual_cortex_move_fovea(VisualCortex *visual, float dx, float dy);

#endif
