//
// Created by nitori on 16/03/17.
//
#include "neat.h"


int print_genome(Genome* g) {
    printf("Genome(\n");
    printf("  Number of connections: %d\n", g->connections->size);
    printf("  Connections:\n");

    int i;
    Connection* current;
    for (i=0; i<g->connections->size; i++) {
        current = vector_get(g->connections, i);
        printf("    Connection(%s, [%p], in=(%d, %f, [%p]), out=(%d, %f, %p), inumber=%d, weight=%f)\n",
               current->enabled ? " ON" : "OFF",
               current,
               current->in->id, current->in->value, current->in,
               current->out->id, current->out->value, current->out,
               current->inumber,
               current->weight);
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
        printf("%d %f [%p]", node->id, node->value, node);
    }
}

Genome* new_genome(int32_t input_nodes, int32_t output_nodes) {
    Genome* g = calloc(1, sizeof(Genome));
    g->fitness = 0.0;
    g->connections = new_vector();
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

void delta_genomes(DeltaResult* result, Genome* g1, Genome* g2, double coeff_d, double coeff_w) {
    if (g1->connections->size == 0 || g2->connections->size == 0) {
        fprintf(stderr, "Genomes have to be non-empty to calculate delta.\n");
        return;
    }
    int32_t max_count = g1->connections->size > g2->connections->size ? g1->connections->size : g2->connections->size;
    double weight_diff = 0.0;
    int32_t disjoint_count = 0;

    int n1 = 0, n2 = 0;

    while (n1 < g1->connections->size && n2 < g2->connections->size) {
        Connection* c1 = vector_get(g1->connections, n1);
        Connection* c2 = vector_get(g2->connections, n2);
        if (c1->inumber < c2->inumber) {
            disjoint_count++;
            n1++;
        } else if (c2->inumber < c1->inumber) {
            disjoint_count++;
            n2++;
        } else {
            weight_diff += fabs(c1->weight - c2->weight);
            n1++;
            n2++;
        }
    }

    result->disjoint_count = disjoint_count;
    result->disjoint = (coeff_d * (double)disjoint_count) / (double)max_count;
    result->avg_weight_diff = coeff_w * (weight_diff / (double)max_count);
    result->delta = result->disjoint + result->avg_weight_diff;
}

Genome* clone_genome(Genome* g) {
    Genome* clone = calloc(1, sizeof(*g));
    memcpy(clone, g, sizeof(*g));

    clone->connections = new_vector();
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

    Connection* conn_clone;
    for (i=0; i<g->connections->size; i++) {
        conn_clone = clone_connection(vector_get(g->connections, i));
        conn_clone->in = find_node_in_genome(clone, conn_clone->in->id);
        conn_clone->out = find_node_in_genome(clone, conn_clone->out->id);
        add_connection(clone, conn_clone);
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
    if (g->connections->size == 0) {
        return -1;
    }
    int r = rand() % g->connections->size;
    Connection* c = vector_get(g->connections, r);
    while (!c->enabled) {
        r = rand() % g->connections->size;
        c = vector_get(g->connections, r);
    }

    c->enabled = false;

    Node* node = new_node();

    Connection* c1 = new_connection(1.0, true);
    Connection* c2 = new_connection(c->weight, true);
    connect(c1, c->in, node);
    connect(c2, node, c->out);

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

    Connection* c = new_connection(1.0, true);
    connect(c, node1, node2);
    add_connection(g, c);

    return 0;
}

Genome* mate(Genome* g1, Genome* g2) {
    Genome* offspring = new_genome(0, 0);
    // TODO: make this work

    return offspring;
}

void set_genome_inputs(Genome* g, Vector* inputs) {
    if (g->inputs->size != inputs->size) {
        fprintf(stderr, "Genome inputs vector and supplied inputs are not of same size.\n");
        exit(-1);
    }

    int i;
    double* value;
    Node* node;
    for (i=0; i<inputs->size; i++) {
        node = vector_get(g->inputs, i);
        value = vector_get(inputs, i);
        node->value = *value;
    }
}

void evaluate_genome(Genome* g) {

}

void free_genome(Genome* g) {
    int i;
    for (i=0; i<g->connections->size; i++) {
        free_connection(vector_get(g->connections, i));
    }
    vector_free(g->connections);
    free_nodes(g->inputs);
    free_nodes(g->hidden);
    free_nodes(g->outputs);
    vector_free(g->inputs);
    vector_free(g->hidden);
    vector_free(g->outputs);
    free(g);
}

void save_genome(Genome* g, const char* filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    write_nodes(f, "inputs", g->inputs);
    write_nodes(f, "hidden", g->hidden);
    write_nodes(f, "outputs", g->outputs);

    fprintf(f, "connections:\n");
    int i;
    Connection* c;
    for (i=0; i<g->connections->size; i++) {
        c = vector_get(g->connections, i);
        fprintf(f, "%d %s %f %d %d\n",
                c->inumber,
                c->enabled ? "on" : "off",
                c->weight,
                c->in->id,
                c->out->id);
    }

    fclose(f);
}

void write_nodes(FILE *f, const char* name, Vector* nodes) {
    int i;
    Node* node;
    fprintf(f, "%s = ", name);
    for (i=0; i<nodes->size; i++) {
        node = vector_get(nodes, i);
        if (i > 0) {
            fprintf(f, ", ");
        }
        fprintf(f, "%d", node->id);
    }
    fprintf(f, "\n");
}
