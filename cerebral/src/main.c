#include "cerebral.h"
#include "brain_modules.h"
#include "world_model.h"
#include <stdio.h>

int main() {
    printf("--- Iniciando Simulación de Aprendizaje en Tiempo Real (Cerebral) ---\n");

    // 1. Inicializar red con 100 neuronas
    CerebralNetwork *net = cerebral_init(100);
    WorldModel *wm = world_model_init(10);

    // 2. Definir neuronas: 10 (Estímulo) y 20 (Acción/Efecto)
    world_model_add_concept(wm, 10, "Estímulo");
    world_model_add_concept(wm, 20, "Acción");

    // Conectar Estímulo -> Acción (peso inicial bajo)
    net->neurons[10].synapses[0].target_neuron_id = 20;
    net->neurons[10].synapses[0].weight = 0.5f;
    net->neurons[10].synapse_count = 1;

    printf("Estado inicial: Peso conexión Estímulo -> Acción: %.2f\n", net->neurons[10].synapses[0].weight);

    // 3. Ciclo de aprendizaje
    for (int ciclo = 1; ciclo <= 10; ciclo++) {
        // Estimular entrada
        cerebral_stimulate(net, 10, 1.5f); // Suficiente para disparar neurona 10

        cerebral_tick(net); // T=0.1: Dispara 10

        // Estimular salida manualmente (simulando que la máquina "acierta" o es guiada)
        // Esto crea la correlación temporal necesaria para STDP
        cerebral_stimulate(net, 20, 1.5f);

        cerebral_tick(net); // T=0.2: Dispara 20

        // Aplicar recompensa para potenciar el aprendizaje
        basal_ganglia_update(net, 1.0f);

        if (ciclo % 2 == 0) {
            printf("Ciclo %d: Peso conexión: %.3f (Dopamina: %.2f)\n",
                   ciclo, net->neurons[10].synapses[0].weight, net->global_dopamine);
        }

        hippocampus_consolidate(net);
    }

    printf("\n--- Simulación Finalizada ---\n");
    printf("El peso ha aumentado gracias a la activación conjunta y la recompensa.\n");

    world_model_free(wm);
    cerebral_free(net);
    return 0;
}
