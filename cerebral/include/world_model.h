#ifndef WORLD_MODEL_H
#define WORLD_MODEL_H

#include "cerebral.h"

typedef struct {
    uint32_t neuron_id;
    char concept_name[32];
} Concept;

typedef struct {
    Concept *concepts;
    uint32_t concept_count;
    uint32_t capacity;
} WorldModel;

WorldModel* world_model_init(uint32_t capacity);
void world_model_add_concept(WorldModel *wm, uint32_t nid, const char *name);
void world_model_perceive(CerebralNetwork *net, WorldModel *wm, const char *input);
void world_model_free(WorldModel *wm);

#endif
