//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_CONNECTION_H
#define NEAT_CONNECTION_H

#include "types.h"

struct Link {
    Neuron* in;
    Neuron* out;
    double weight;
    bool enabled;
    INumber inumber;
};


Link* new_link();
void link_init(Link* link);

#endif //NEAT_CONNECTION_H
