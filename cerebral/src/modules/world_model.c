#include "world_model.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

WorldModel* world_model_init(uint32_t capacity) {
    WorldModel *wm = malloc(sizeof(WorldModel));
    if (!wm) return NULL;
    wm->concepts = calloc(capacity, sizeof(Concept));
    if (!wm->concepts) {
        free(wm);
        return NULL;
    }
    wm->concept_count = 0;
    wm->capacity = capacity;
    return wm;
}

void world_model_add_concept(WorldModel *wm, uint32_t nid, const char *name) {
    if (!wm || wm->concept_count >= wm->capacity) {
        fprintf(stderr, "Error: WorldModel capacity exceeded or null pointer\n");
        return;
    }
    wm->concepts[wm->concept_count].neuron_id = nid;

    // Secure string copy with null termination guarantee
    strncpy(wm->concepts[wm->concept_count].concept_name, name, 31);
    wm->concepts[wm->concept_count].concept_name[31] = '\0';

    wm->concept_count++;
}

void world_model_perceive(CerebralNetwork *net, WorldModel *wm, const char *input) {
    if (!net || !wm || !input) return;
    for (uint32_t i = 0; i < wm->concept_count; i++) {
        if (strncmp(wm->concepts[i].concept_name, input, 31) == 0) {
            cerebral_stimulate(net, wm->concepts[i].neuron_id, 1.2f);
        }
    }
}

void world_model_free(WorldModel *wm) {
    if (wm) {
        if (wm->concepts) free(wm->concepts);
        free(wm);
    }
}
