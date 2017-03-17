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
        printf("    Connection(%s, in=(%d, %f, %p), out=(%d, %f, %p), inumber=%d, weight=%f)\n",
               current->enabled ? " ON" : "OFF",
               current->in->id, current->in->value, current->in,
               current->out->id, current->out->value, current->out,
               current->inumber,
               current->weight);
        current = current->next;
    }

    printf("  Nodes:\n");

    printf("    Inputs (%d): ", g->inputs->size);
    print_nodes(g->inputs);
    printf("\n");

    printf("    Hidden (%d): ", g->hidden->size);
    print_nodes(g->hidden);
    printf("\n");

    printf("    Outputs (%d): ", g->outputs->size);
    print_nodes(g->outputs);
    printf("\n");

    printf(")\n");
    return 0;
}

void print_nodes(Vector* nodes) {
    int i;
    Node* node;
    for (i=0; i < nodes->size; i++) {
        node = vector_get(nodes, i);
        if (i > 0) {
            printf(", ");
        }
        printf("%d [%p]", node->id, node);
    }
}

Genome* init_genome(int32_t input_nodes, int32_t output_nodes) {
    Genome* g = calloc(1, sizeof(Genome));
    g->conn_count = 0;
    g->fitness = 0.0;
    g->head = NULL;
    g->tail = NULL;
    g->inputs = new_vector();
    g->outputs = new_vector();
    g->hidden = new_vector();

    int i;
    for (i=0; i<input_nodes; i++) {
        vector_append(g->inputs, new_node());
    }
    for (i=0; i<output_nodes; i++) {
        vector_append(g->outputs, new_node());
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

Genome* clone_genome(Genome* g) {
    Genome* clone = calloc(1, sizeof(*g));
    memcpy(clone, g, sizeof(*g));

    clone->head = NULL;
    clone->tail = NULL;
    clone->conn_count = 0;

    clone->inputs = new_vector();
    clone->hidden = new_vector();
    clone->outputs = new_vector();
    int i;
    for (i=0; i<g->inputs->size; i++) {
        vector_append(clone->inputs, clone_node(vector_get(g->inputs, i)));
    }
    for (i=0; i<g->hidden->size; i++) {
        vector_append(clone->hidden, clone_node(vector_get(g->hidden, i)));
    }
    for (i=0; i<g->outputs->size; i++) {
        vector_append(clone->outputs, clone_node(vector_get(g->outputs, i)));
    }

    Connection* current = g->head;
    Connection* c;
    while (current) {
        c = clone_connection(current);
        c->in = find_node_in_genome(clone, c->in->id);
        c->out = find_node_in_genome(clone, c->out->id);
        add_connection(clone, c);
        current = current->next;
    }

    return clone;
}

Node* find_node_in_genome(Genome* g, int32_t id) {
    Node* node;
    node = find_node_in_vector(g->inputs, id);
    if (node != NULL) {
        return node;
    }
    node = find_node_in_vector(g->hidden, id);
    if (node != NULL) {
        return node;
    }
    node = find_node_in_vector(g->outputs, id);
    if (node != NULL) {
        return node;
    }
    return NULL;
}

Node* find_node_in_vector(Vector* v, int32_t id) {
    int i;
    Node* node;
    for (i=0; i<v->size; i++) {
        node = vector_get(v, i);
        if (node->id == id) {
            return node;
        }
    }
    return NULL;
}


int mutate_split_connection(Genome* g) {
    if (g->conn_count == 0) {
        return -1;
    }
    int r = rand() % g->conn_count;
    Connection* c = get_connection(g, r);
    while (!c->enabled) {
        r = rand() % g->conn_count;
        c = get_connection(g, r);
    }

    c->enabled = false;

    Node* node = new_node();

    Connection* c1 = init_connection(c->in, node, 1.0, true);
    Connection* c2 = init_connection(node, c->out, c->weight, true);

    add_connection(g, c1);
    add_connection(g, c2);
    vector_append(g->hidden, node);

    return 0;
}

int mutate_connect(Genome* g) {
    // connect either
    // input -> output
    // input -> hidden
    // hidden -> output
    // hidden -> hidden

    int in_index = rand() % (g->inputs->size + g->hidden->size);
    int out_index = rand() % (g->hidden->size + g->outputs->size);

    Node *node1;
    Node *node2;

    if (in_index < g->inputs->size) {
        node1 = vector_get(g->inputs, in_index);
    } else {
        node1 = vector_get(g->hidden, in_index - g->inputs->size);
    }
    if (out_index < g->hidden->size) {
        node2 = vector_get(g->hidden, out_index);
    } else {
        node2 = vector_get(g->outputs, out_index - g->hidden->size);
    }

    Connection* c = init_connection(node1, node2, 1.0, true);
    add_connection(g, c);

    return 0;
}

Genome* mate(Genome* g1, Genome* g2) {
    Genome* offspring = init_genome(0, 0);
    // TODO: make this work

    return offspring;
}

void free_genome(Genome* g) {
    Connection* current = g->head;
    while (current) {
        free_connection(current);
        current = current->next;
    }
    int i;
    free_nodes(g->inputs);
    free_nodes(g->hidden);
    free_nodes(g->outputs);
    vector_free(g->inputs);
    vector_free(g->hidden);
    vector_free(g->outputs);
    free(g);
}
