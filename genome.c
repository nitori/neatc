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
        printf("    Connection(%s, inumber=%d, weight=%f)\n",
               current->enabled ? "ON" : "OFF",
               current->inumber,
               current->weight);
        current = current->next;
    }

    printf("  Number of nodes: %d\n", g->node_count);
    printf("  Nodes:\n");

    Node* currentNode = g->node_head;
    while (currentNode) {
        printf("    Node(type=%d)\n", currentNode->type);
        currentNode = currentNode->next;
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

    Node *tmp_node;
    int i;
    for (i=0; i<input_nodes; i++) {
        tmp_node = init_node();
        tmp_node->type = inputNode;
        add_node(g, tmp_node);
    }
    for (i=0; i<output_nodes; i++) {
        tmp_node = init_node();
        tmp_node->type = outputNode;
        add_node(g, tmp_node);
    }
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
