//
// Created by nitori on 16/03/17.
//
#include "neat.h"


Node* init_node() {
    Node* n = calloc(1, sizeof(Node));
    n->type = hiddenNode;  // default
    return n;
}

int add_node(Genome* g, Node* node) {
    if (node->owner != NULL) {
        fprintf(stderr, "Node is already owned by another genome.\n");
        return -1;
    }
    node->owner = g;
    if (g->node_tail == NULL) {
        g->node_tail = node;
        g->node_head = node;
        node->next = NULL;
        node->prev = NULL;
    } else {
        node->next = NULL;
        node->prev = g->node_tail;
        g->node_tail->next = node;
        g->node_tail = node;
    }
    g->node_count++;
    return 0;
}

int add_hidden_node(Genome* g, Node* node) {
    if (node->owner != NULL) {
        fprintf(stderr, "Node is already owned by another genome.\n");
        return -1;
    }
    node->owner = g;
    node->type = hiddenNode;
    Node* current = g->node_head;
    while (current) {
        if (current->type == outputNode) {
            // first output node. add before the current.
            node->next = current;
            node->prev = current->prev;
            current->prev->next = node;
            current->prev = node;
            break;
        }
        current = current->next;
    }
    g->node_count++;
    return 0;
}

Node* get_node(Genome* g, int32_t index) {
    if (index >= g->node_count) {
        return NULL;
    }
    Node* current = g->node_head;
    int i = 0;
    while (current) {
        if (index == i) {
            return current;
        }
        i++;
        current = current->next;
    }
    return NULL;
}
