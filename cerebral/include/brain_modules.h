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

// The Locus Coeruleus triggers Noradrenaline on novelty
void locus_coeruleus_update(CerebralNetwork *net, bool novelty_detected);

// The ACC detects conflict between firing neurons
float acc_detect_conflict(CerebralNetwork *net, uint32_t *neuron_ids, uint32_t count);

// Association Cortex connects concepts spontaneously
void association_cortex_connect(CerebralNetwork *net, uint32_t nid_a, uint32_t nid_b, float initial_weight);

// Thalamus gates sensory input
float thalamus_gate_input(CerebralNetwork *net, float raw_input, float attention_signal);

#endif
