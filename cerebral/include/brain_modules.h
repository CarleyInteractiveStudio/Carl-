#ifndef BRAIN_MODULES_H
#define BRAIN_MODULES_H

#include "cerebral.h"

// The PFC regulates attention and modulates neural excitability
void pfc_process(CerebralNetwork *net, uint32_t *active_goal_neurons, uint32_t count);

// The Reflex system bypasses the PFC for immediate action
void reflex_trigger(CerebralNetwork *net, uint32_t input_id, uint32_t action_id);

// The Hippocampus manages memory consolidation
void hippocampus_consolidate(CerebralNetwork *net);

// The Basal Ganglia processes rewards and updates dopamine
void basal_ganglia_update(CerebralNetwork *net, float reward_signal);

#endif
