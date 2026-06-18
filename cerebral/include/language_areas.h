#ifndef LANGUAGE_AREAS_H
#define LANGUAGE_AREAS_H

#include "cerebral.h"

// Wernicke's Area handles semantic understanding (Mapping spikes to meaning)
void wernicke_process(CerebralNetwork *net, uint32_t *word_spike_sequence, uint32_t length);

// Broca's Area handles speech production and syntax (Sequencing spikes for output)
void broca_generate(CerebralNetwork *net, uint32_t concept_nid, uint32_t *output_sequence, uint32_t *out_length);

#endif
