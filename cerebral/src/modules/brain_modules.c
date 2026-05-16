#include "brain_modules.h"
#include <stdio.h>

void pfc_process(CerebralNetwork *net, uint32_t *active_goal_neurons, uint32_t count) {
    // In a real brain, the PFC provides top-down excitation to goal-relevant areas
    for (uint32_t i = 0; i < count; i++) {
        uint32_t nid = active_goal_neurons[i];
        if (nid < net->total_neurons) {
            // Slightly boost the potential of neurons related to current "goals"
            net->neurons[nid].membrane_potential += 0.05f;
        }
    }
}

void basal_ganglia_update(CerebralNetwork *net, float reward_signal) {
    if (!net) return;

    // Reward signal (0.0 to 1.0) slowly shifts the global dopamine level
    // This simulates "learning from experience"
    float alpha = 0.1f; // Learning rate for dopamine
    net->global_dopamine = (1.0f - alpha) * net->global_dopamine + alpha * reward_signal;

    // Decay dopamine over time towards a neutral state (0.5)
    net->global_dopamine = 0.99f * net->global_dopamine + 0.01f * 0.5f;
}

void locus_coeruleus_update(CerebralNetwork *net, bool novelty_detected) {
    if (!net) return;

    if (novelty_detected) {
        // Sudden spike in noradrenaline
        net->global_noradrenaline += 0.3f;
        if (net->global_noradrenaline > 1.0f) net->global_noradrenaline = 1.0f;
    }

    // Natural decay of alertness
    // Natural decay of alertness
    net->global_noradrenaline *= 0.98f;
    if (net->global_noradrenaline < 0.1f) net->global_noradrenaline = 0.1f;
}

float acc_detect_conflict(CerebralNetwork *net, uint32_t *neuron_ids, uint32_t count) {
    if (!net || count < 2) return 0.0f;

    uint32_t firing_count = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (net->neurons[neuron_ids[i]].has_spiked) {
            firing_count++;
        }
    }

    // High conflict if multiple mutually exclusive neurons fire at the same time
    if (firing_count > 1) {
        return (float)firing_count / (float)count;
    }
    return 0.0f;
}

void association_cortex_connect(CerebralNetwork *net, uint32_t nid_a, uint32_t nid_b, float initial_weight) {
    if (!net || nid_a >= net->total_neurons || nid_b >= net->total_neurons) return;

    Neuron *a = &net->neurons[nid_a];
    if (a->synapse_count < MAX_SYNAPSES_PER_NEURON) {
        a->synapses[a->synapse_count].target_neuron_id = nid_b;
        a->synapses[a->synapse_count].weight = initial_weight;
        a->synapses[a->synapse_count].last_spike_time = -100.0f;
        a->synapse_count++;
    }
}

float thalamus_gate_input(CerebralNetwork *net, float raw_input, float attention_signal) {
    // The thalamus multiplies the sensory input by the attention signal
    // simulating selective processing of information.
    return raw_input * (0.2f + 0.8f * attention_signal);
}

void cerebral_sleep(CerebralNetwork *net) {
    if (!net || net->trace_count == 0) return;

    printf("[SUEÑO] Iniciando consolidación de %u trazas...\n", net->trace_count);

    // 1. Replay: Re-activar trazas de memoria para fortalecer sinapsis activas recientemente
    // En un cerebro real esto ocurre en ráfagas (sharp-wave ripples)
    for (uint32_t i = 0; i < net->trace_count; i++) {
        cerebral_stimulate(net, net->traces[i].neuron_id, 0.5f);
        cerebral_tick(net);
    }

    // 2. Pruning: Eliminar sinapsis muy débiles o irrelevantes para optimizar recursos
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        Neuron *n = &net->neurons[i];
        for (uint32_t j = 0; j < n->synapse_count; j++) {
            if (n->synapses[j].weight < 0.01f && n->synapses[j].weight > -0.01f) {
                // Mover la última sinapsis a esta posición para "eliminar" la débil
                n->synapses[j] = n->synapses[n->synapse_count - 1];
                n->synapse_count--;
                j--;
            }
        }
    }

    // 3. Resetear trazas para el próximo día/ciclo
    net->trace_count = 0;
}

void hypothalamus_update(CerebralNetwork *net) {
    if (!net) return;

    // Aumenta el hambre de curiosidad con el tiempo si no hay noradrenalina (novedad)
    if (net->global_noradrenaline < 0.3f) {
        net->curiosity_drive += 0.001f;
    } else {
        net->curiosity_drive -= 0.01f;
    }

    // El hambre de coherencia aumenta si hay conflicto detectado por el ACC
    // (Simulado aquí, pero se conectaría al output del acc_detect_conflict)

    // Limitar drives
    if (net->curiosity_drive > 1.0f) net->curiosity_drive = 1.0f;
    if (net->curiosity_drive < 0.0f) net->curiosity_drive = 0.0f;

    // La curiosidad alta aumenta la noradrenalina base (el motor busca estímulos)
    if (net->curiosity_drive > 0.8f) {
        net->global_noradrenaline += 0.01f;
    }
}

void pfc_simulate_prospect(CerebralNetwork *net, uint32_t concept_nid) {
    if (!net) return;

    printf("[PFC] Imaginando futuro para concepto %u...\n", concept_nid);

    // Entrar en modo simulación (off-line)
    net->is_simulating = true;
    float original_dopamine = net->global_dopamine;

    // Estimular el concepto internamente
    cerebral_stimulate(net, concept_nid, 1.2f);

    // Dejar que la red se propague unos pasos
    for (int i = 0; i < 5; i++) {
        cerebral_tick(net);
    }

    // Salir de modo simulación
    net->is_simulating = false;
    net->global_dopamine = original_dopamine;
}

void reflex_trigger(CerebralNetwork *net, uint32_t input_id, uint32_t action_id) {
    // Reflexes are strong, direct connections
    if (net->neurons[input_id].has_spiked) {
        // Force a spike in the action neuron (instinct)
        net->neurons[action_id].membrane_potential = SPIKE_THRESHOLD + 0.1f;
    }
}

void hippocampus_consolidate(CerebralNetwork *net) {
    // Move short-term synaptic changes to long-term "stability"
    // For now, we simulate this by normalizing weights to prevent explosion/decay
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        for (uint32_t j = 0; j < net->neurons[i].synapse_count; j++) {
            Synapse *s = &net->neurons[i].synapses[j];
            if (s->weight > 5.0f) s->weight = 5.0f;
            if (s->weight < -1.0f) s->weight = -1.0f;
        }
    }
}
