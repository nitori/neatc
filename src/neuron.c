//
// Created by nitori on 18.03.17.
//

#include "neuron.h"
#include "innovation.h"
#include <stdlib.h>

Neuron* new_neuron() {
    return calloc(1, sizeof(Neuron));
}

void neuron_init(Neuron* neuron) {
    neuron->id = next_inumber();
    neuron->type = NormalNeuron;
    neuron->level = 0;
    neuron->value = 0.0;
}

void neuron_free(Neuron* neuron) {

}
