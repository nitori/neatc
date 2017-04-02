//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_INNOVATION_H
#define NEAT_INNOVATION_H

#include "types.h"


typedef enum InnovationType {
    InnovationLinkType,
    InnovationNeuronType,
} InnovationType;

typedef struct LinkInnovation {
    NeuronId neuron_in_id;
    NeuronId neuron_out_id;
    INumber new_link_inumber;
} LinkInnovation;

typedef struct NeuronInnovation {
    INumber inumber;
    NeuronId new_neuron_id;
    int32_t new_neuron_level;
    INumber new_link_in_inumber;
    INumber new_link_out_inumber;
} NeuronInnovation;

typedef struct Innovation {
    InnovationType type;
    union {
        LinkInnovation link;
        NeuronInnovation neuron;
    } innovation;
} Innovation;


Innovation* new_innovation();
void innovation_init(Innovation* innovation);

Innovation* new_link_innovation(
        NeuronId neuron_in_id,
        NeuronId neuron_out_id,
        INumber new_link_inumber);

Innovation* new_neuron_innovation(
        INumber inumber,
        NeuronId new_neuron_id,
        int32_t new_neuron_level,
        INumber new_link_in_inumber,
        INumber new_link_out_inumber);

INumber next_inumber();

#endif //NEAT_INNOVATION_H
