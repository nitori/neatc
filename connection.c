//
// Created by nitori on 16/03/17.
//
#include "neat.h"


Connection* init_connection(Node* in, Node* out,
                            double weight, bool enabled) {
    Connection* c = calloc(1, sizeof(Connection));
    if (c == NULL) {
        return NULL;
    }
    c->in = in;
    c->out = out;
    c->weight = weight;
    c->enabled = enabled;
    c->inumber = next_inumber();
    return c;
}

int add_connection(Genome* g, Connection* c) {
    if (c->owner != NULL) {
        fprintf(stderr, "Connection is already owned by another genome.\n");
        return -1;
    }
    c->owner = g;
    vector_append(g->connections, c);
    return 0;
}

Connection* clone_connection(Connection* c) {
    Connection* new_c = calloc(1, sizeof(*c));
    memcpy(new_c, c, sizeof(*c));
    new_c->owner = NULL;
    return new_c;
}

Connection* find_connection(Genome* g, int32_t inumber) {
    Connection* current;
    int i;
    for (i=0; i<g->connections->size; i++) {
        current = vector_get(g->connections, i);
        if (current->inumber == inumber) {
            return current;
        } else if (current->inumber > inumber) {
            return NULL;
        }
    }
    return NULL;
}

void free_connection(Connection* c) {
    free(c);
}
