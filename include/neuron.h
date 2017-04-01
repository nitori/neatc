//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_NEURON_H
#define NEAT_NEURON_H

#include "list.h"
#include "types.h"

typedef enum NeuronType {
    NormalNeuron,
    BiasNeuron,
} NeuronType;

typedef struct Neuron {
    NeuronId id;
    NeuronType type;
    int32_t level;
    double value;
    List inputs;
} Neuron;

Neuron* new_neuron();
void neuron_init(Neuron* neuron);

#endif //NEAT_NEURON_H
