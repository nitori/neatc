//
// Created by nitori on 16.03.17.
//

#include "neat.h"

Node* new_node() {
    static int32_t global_node_counter = 0;
    global_node_counter++;
    Node* n = calloc(1, sizeof(Node));
    n->id = global_node_counter;
    n->level = (rand() % MAX_HIDDEN_LEVELS) + 1;
    // inputs = 0
    // outputs = MAX_HIDDEN_LEVELS + 1
    return n;
}

Node* clone_node(Node* node) {
    Node* new_n = calloc(1, sizeof(*node));
    memcpy(new_n, node, sizeof(*node));
    return new_n;
}

void free_node(Node* node) {
    free(node);
}

void free_nodes(Vector* v) {
    int i;
    for (i=0; i<v->size; i++) {
        free_node(vector_get(v, i));
    }
}
