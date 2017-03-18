//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_NEURON_H
#define NEAT_NEURON_H

#include "types.h"

enum NeuronType {
    NormalNeuron,
    BiasNeuron,
};

struct Neuron {
    NeuronId id;
    NeuronType type;
    double value;
    List inputs;
};

Neuron* new_neuron();
void neuron_init(Neuron* neuron);

#endif //NEAT_NEURON_H
