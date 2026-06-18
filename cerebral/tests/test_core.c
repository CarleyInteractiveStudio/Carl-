#include "cerebral.h"
#include <stdio.h>
#include <assert.h>

void test_neuron_spike() {
    CerebralNetwork *net = cerebral_init(2);

    // Connect neuron 0 to neuron 1
    net->neurons[0].synapses[0].target_neuron_id = 1;
    net->neurons[0].synapses[0].weight = 1.1f; // High enough to trigger spike in target
    net->neurons[0].synapse_count = 1;

    // Stimulate neuron 0
    cerebral_stimulate(net, 0, 1.5f);

    // Tick 1: Neuron 0 should spike
    cerebral_tick(net);
    assert(net->neurons[0].has_spiked == true);

    // Tick 2: Spike should have propagated to neuron 1
    // (In our simple model, propagation happens in the same tick if coded so,
    // but let's check current logic: tick handles spikes then propagation)

    // After tick 1, neuron 1 should have received the weight
    assert(net->neurons[1].membrane_potential >= 1.1f);

    // Tick 2: Neuron 1 should spike
    cerebral_tick(net);
    assert(net->neurons[1].has_spiked == true);

    cerebral_free(net);
    printf("Test 'test_neuron_spike' passed!\n");
}

int main() {
    test_neuron_spike();
    return 0;
}
