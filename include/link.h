//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_CONNECTION_H
#define NEAT_CONNECTION_H

#include "neuron.h"

#include "types.h"

typedef struct Link {
    Neuron* in;
    Neuron* out;
    double weight;
    bool enabled;
    INumber inumber;
} Link;


Link* new_link();
void link_init(Link* link);

void link_free(Link* link);

#endif //NEAT_CONNECTION_H
