//
// Created by nitori on 16.03.17.
//

#include "neat.h"

Node* new_node() {
    static int32_t global_node_counter = 0;
    global_node_counter++;
    Node* n = calloc(1, sizeof(Node));
    n->outputs = new_vector();
    n->id = global_node_counter;
    return n;
}

Node* clone_node(Node* node) {
    Node* new_n = calloc(1, sizeof(*node));
    memcpy(new_n, node, sizeof(*node));
    return new_n;
}

void free_node(Node* node) {
    free(node);
    vector_free(node->outputs);
}

void free_nodes(Vector* v) {
    int i;
    for (i=0; i<v->size; i++) {
        free_node(vector_get(v, i));
    }
}
