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
    if (g->tail == NULL) {
        g->tail = c;
        g->head = c;
        c->next = NULL;
        c->prev = NULL;
    } else {
        c->next = NULL;
        c->prev = g->tail;
        g->tail->next = c;
        g->tail = c;
    }
    g->conn_count++;
    return 0;
}

Connection* get_connection(Genome* g, int32_t index) {
    if (index >= g->conn_count) {
        return NULL;
    }
    Connection* current = g->head;
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

Connection* clone_connection(Connection* c) {
    Connection* new_c = calloc(1, sizeof(*c));
    memcpy(new_c, c, sizeof(*c));
    new_c->owner = NULL;
    return new_c;
}

Connection* find_connection(Genome* g, int32_t inumber) {
    Connection* current = g->head;
    while (current) {
        if (current->inumber == inumber) {
            return current;
        } else if (current->inumber > inumber) {
            return NULL;
        }
        current = current->next;
    }
    return NULL;
}

void free_connection(Connection* c) {
    free(c);
}
