//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_CONNECTION_H
#define NEAT_CONNECTION_H

#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "innovation.h"

typedef struct Link Link;

struct Link {
    //Node* in;
    //Node* out;
    double weight;
    bool enabled;
    INumber inumber;
};


Link* new_link();
void link_init(Link* link, double weight, bool enabled);
void link_free(Link *c);

#endif //NEAT_CONNECTION_H
