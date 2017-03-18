//
// Created by nitori on 16/03/17.
//
#include "neat.h"


Link* new_link(double weight, bool enabled) {
    Link* c = calloc(1, sizeof(Link));
    if (c == NULL) {
        return NULL;
    }
    c->in = NULL;
    c->out = NULL;
    c->weight = weight;
    c->enabled = enabled;
    c->inumber = next_inumber();
    return c;
}

void connect(Link* c, Node* in, Node* out) {
    c->in = in;
    c->out = out;
}

int add_link(Genome *g, Link *c) {
    if (c->owner != NULL) {
        fprintf(stderr, "Link is already owned by another genome.\n");
        return -1;
    }
    c->owner = g;
    vector_append(g->links, c);
    return 0;
}

Link* clone_link(Link *c) {
    Link* new_c = calloc(1, sizeof(*c));
    memcpy(new_c, c, sizeof(*c));
    new_c->owner = NULL;
    return new_c;
}

Link* find_link(Genome *g, int32_t inumber) {
    Link* current;
    int i;
    for (i=0; i<g->links->size; i++) {
        current = vector_get(g->links, i);
        if (current->inumber == inumber) {
            return current;
        } else if (current->inumber > inumber) {
            return NULL;
        }
    }
    return NULL;
}

void free_link(Link *c) {
    free(c);
}
