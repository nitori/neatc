//
// Created by nitori on 14.03.17.
//
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "neat.h"

int32_t next_inumber() {
    static int global_inumber = 0;
    global_inumber++;
    return global_inumber;
}

int print_genome(Genome* g) {
    printf("Genome(\n");
    if (g->head == NULL) {
        printf("    No Connections initialized\n)\n");
        return -1;
    }
    printf("  Number of connections: %d\n", g->conn_count);
    printf("  Connections:\n");

    ConnectionGene* current = g->head;
    while (current) {
        printf("    Connection(%s, inumber=%d, weight=%f)\n",
               current->enabled ? "ON" : "OFF",
               current->inumber,
               current->weight);
        current = current->next;
    }
    printf(")\n");
    return 0;
}

Genome* init_genome(int32_t input_nodes, int32_t output_nodes) {
    Genome* g = calloc(1, sizeof(Genome));
    g->conn_count = 0;
    g->fitness = 0.0;
    g->head = NULL;
    g->tail = NULL;
    return g;
}

NodeGene* init_node() {
    NodeGene* n = calloc(1, sizeof(NodeGene));
    return n;
}

ConnectionGene* init_connection(NodeGene* in, NodeGene *out,
                    double weight, bool enabled) {
    ConnectionGene* c = calloc(1, sizeof(ConnectionGene));
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

int add_node(Genome* g, ConnectionGene* c_old) {
    NodeGene* new_node = NULL;
    ConnectionGene* c1 = init_connection(c_old->in, new_node, c_old->weight, true);
    ConnectionGene* c2 = init_connection(new_node, c_old->out, c_old->weight, true);
    c_old->enabled = false;
    add_connection(g, c1);
    add_connection(g, c2);
    return 0;
}

int add_connection(Genome* g, ConnectionGene* c) {
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

ConnectionGene* clone_connection(ConnectionGene* c) {
    ConnectionGene* new_c = calloc(1, sizeof(*c));
    memcpy(new_c, c, sizeof(*c));
    new_c->owner = NULL;
    return new_c;
}


ConnectionGene* find_connection(Genome* g, int32_t inumber) {
    ConnectionGene* current = g->head;
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

DeltaResult* delta_genomes(Genome* g1, Genome* g2, double coeff_d, double coeff_e, double coeff_w) {
    ConnectionGene* cmin1 = g1->head;
    ConnectionGene* cmin2 = g2->head;
    ConnectionGene* cmin = cmin2->inumber < cmin1->inumber ? cmin2 : cmin1;

    ConnectionGene* cmax1 = g1->tail;
    ConnectionGene* cmax2 = g2->tail;
    ConnectionGene* cmax = cmax2->inumber > cmax1->inumber ? cmax2 : cmax1;

    int32_t max_count = g1->conn_count > g2->conn_count ? g1->conn_count : g2->conn_count;
    double weight_diff = 0.0;
    int32_t disjoint_count = 0;
    int32_t excess_count = 0;

    ConnectionGene *c1, *c2;
    int i;

    for (i=cmin->inumber; i<=cmax->inumber; i++) {
        c1 = find_connection(g1, i);
        c2 = find_connection(g2, i);
        if (c1 != NULL && c2 != NULL) {
            weight_diff += fabs(c1->weight - c2->weight);
        } else if(c1 == NULL && c2 == NULL) {
            continue;
        } else if(c1 == NULL) {
            if (c2->inumber > cmax1->inumber || c2->inumber < cmin1->inumber) {
                excess_count++;
            } else {
                disjoint_count++;
            }
        } else {
            if (c1->inumber > cmax2->inumber || c1->inumber < cmin2->inumber) {
                excess_count++;
            } else {
                disjoint_count++;
            }
        }
    }

    DeltaResult* result = calloc(1, sizeof(DeltaResult));
    result->disjoint_count = disjoint_count;
    result->disjoint = (coeff_d * (double)disjoint_count) / (double)max_count;
    result->excess_count = excess_count;
    result->excess = (coeff_e * (double)excess_count) / (double)max_count;
    result->avg_weight_diff = coeff_w * (weight_diff / (double)max_count);
    result->delta = result->disjoint + result->excess + result->avg_weight_diff;
    return result;
}
