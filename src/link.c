//
// Created by nitori on 16/03/17.
//

#include "link.h"


Link* new_link() {
    return calloc(1, sizeof(Link));
}

void link_init(Link* link, double weight, bool enabled) {
//    link->in = NULL;
//    link->out = NULL;
    link->weight = weight;
    link->enabled = enabled;
    link->inumber = next_inumber();
}

void link_free(Link *c) {
}
