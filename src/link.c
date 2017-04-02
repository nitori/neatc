//
// Created by nitori on 16/03/17.
//
#include "link.h"
#include "innovation.h"
#include <stdlib.h>


Link* new_link() {
    return calloc(1, sizeof(Link));
}

void link_init(Link* link) {
    link->in = NULL;
    link->out = NULL;
    link->weight = (rand() / (1.0 + RAND_MAX)) * 2.0;
    link->enabled = true;
    link->inumber = next_inumber();
}

void link_free(Link* link) {
}
