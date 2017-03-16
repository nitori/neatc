//
// Created by nitori on 16/03/17.
//
#include "neat.h"


int print_genome(Genome* g) {
    printf("Genome(\n");
    printf("  Number of connections: %d\n", g->conn_count);
    printf("  Connections:\n");

    Connection* current = g->head;
    while (current) {
        printf("    Connection(%s, in=%d, out=%d, inumber=%d, weight=%f)\n",
               current->enabled ? "ON" : "OFF",
               current->in,
               current->out,
               current->inumber,
               current->weight);
        current = current->next;
    }

    printf("  Number of input nodes: %d\n", g->input_count);
    printf("  Number of output nodes: %d\n", g->output_count);
    printf("  Number of hidden nodes: %d\n", g->hidden_count);
    printf(")\n");
    return 0;
}

Genome* init_genome(int32_t input_nodes, int32_t output_nodes) {
    Genome* g = calloc(1, sizeof(Genome));
    g->conn_count = 0;
    g->fitness = 0.0;
    g->head = NULL;
    g->tail = NULL;
    g->input_count = input_nodes;
    g->output_count = output_nodes;
    g->hidden_count = 0;
    return g;
}

DeltaResult* delta_genomes(Genome* g1, Genome* g2, double coeff_d, double coeff_e, double coeff_w) {
    Connection* cmin1 = g1->head;
    Connection* cmin2 = g2->head;
    Connection* cmin = cmin2->inumber < cmin1->inumber ? cmin2 : cmin1;

    Connection* cmax1 = g1->tail;
    Connection* cmax2 = g2->tail;
    Connection* cmax = cmax2->inumber > cmax1->inumber ? cmax2 : cmax1;

    int32_t max_count = g1->conn_count > g2->conn_count ? g1->conn_count : g2->conn_count;
    double weight_diff = 0.0;
    int32_t disjoint_count = 0;
    int32_t excess_count = 0;

    Connection *c1, *c2;
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

Genome* clone_genome(Genome* g) {
    Genome* clone = calloc(1, sizeof(*g));
    memcpy(clone, g, sizeof(*g));

    clone->head = NULL;
    clone->tail = NULL;
    clone->conn_count = 0;

    Connection* current = g->head;
    Connection* c;
    while (current) {
        c = clone_connection(current);
        add_connection(clone, c);
        current = current->next;
    }
    return clone;
}

int mutate_split_connection(Genome* g) {
    int r = rand() % g->conn_count;
    Connection* c = get_connection(g, r);
    while (!c->enabled) {
        r = rand() % g->conn_count;
        c = get_connection(g, r);
    }

    c->enabled = false;
    int32_t new_node = g->input_count + g->output_count + g->hidden_count;
    Connection* c1 = init_connection(c->in, new_node, 1.0, true);
    Connection* c2 = init_connection(new_node, c->out, c->weight, true);
    add_connection(g, c1);
    add_connection(g, c2);
    g->hidden_count++;
    return 0;
}

int mutate_connect(Genome* g) {
    // connect either
    // input -> output
    // input -> hidden
    // hidden -> output
    // hidden -> hidden
    int32_t node1 = rand() % (g->input_count + g->hidden_count);
    int32_t node2 = (rand() % (g->hidden_count + g->output_count)) + g->input_count;

    Connection* c = init_connection(node1, node2, (double)rand() / (double)RAND_MAX, true);
    add_connection(g, c);

    return 0;
}

Genome* mate(Genome* g1, Genome* g2) {
    Genome* offspring = init_genome(g1->input_count, g1->output_count);
    Connection* c1 = g1->head;
    Connection* c2 = g2->head;
    Connection* tmpc;

    while (c1 && c2) {
        if (c1->inumber < c2->inumber) {
            tmpc = clone_connection(c1);
            c1 = c1->next;
        } else if (c2->inumber < c1->inumber) {
            tmpc = clone_connection(c2);
            c2 = c2->next;
        } else {
            if (g1->fitness > g2->fitness) {
                tmpc = clone_connection(c1);
            } else {
                tmpc = clone_connection(c2);
            }
            c1 = c1->next;
            c2 = c2->next;
        }
        add_connection(offspring, tmpc);
    }
    Connection* rest = c1 != NULL ? c1 : c2;
    while (rest) {
        add_connection(offspring, clone_connection(rest));
        rest = rest->next;
    }
    return offspring;
}
