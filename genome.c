//
// Created by nitori on 16/03/17.
//
#include "neat.h"


int print_genome(Genome* g) {
    printf("Genome(\n");
    printf("  Number of connections: %d\n", g->connections->size);
    printf("  Connections:\n");

    int i;
    Link* current;
    for (i=0; i<g->connections->size; i++) {
        current = vector_get(g->connections, i);
        printf("    Link(%s, [%p], in=(%d, %f, [%p]), out=(%d, %f, %p), inumber=%d, weight=%f)\n",
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
    g->owner = NULL;

    Node* node;
    int i;
    for (i=0; i<input_nodes; i++) {
        node = new_node();
        node->level = 0;
        vector_append(g->inputs, node);
    }
    for (i=0; i<output_nodes; i++) {
        node = new_node();
        node->level = MAX_HIDDEN_LEVELS + 1;
        vector_append(g->outputs, node);
    }
    return g;
}

void delta_genomes(DeltaResult* result, Genome* g1, Genome* g2, double coeff_d, double coeff_e, double coeff_w) {
    if (g1->connections->size == 0 || g2->connections->size == 0) {
        result->disjoint_count = 0;
        result->disjoint = 0.0;
        result->excess_count = abs(g1->connections->size - g2->connections->size);
        if (result->excess_count > 0) {
            result->excess = (coeff_e * (double) result->excess_count) / (double) result->excess_count;
        } else {
            result->excess = 0.0;
        }
        result->avg_weight_diff = 0.0;
        result->delta = result->excess;
        return;
    }
    int32_t max_count = g1->connections->size > g2->connections->size ? g1->connections->size : g2->connections->size;
    double weight_diff = 0.0;
    int32_t disjoint_count = 0;

    int n1 = 0, n2 = 0;
    Link* c1;
    Link* c2;
    while (n1 < g1->connections->size && n2 < g2->connections->size) {
        c1 = vector_get(g1->connections, n1);
        c2 = vector_get(g2->connections, n2);
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
    if (n1 < g1->connections->size) {
        result->excess_count = g1->connections->size - n1;
    } else {
        result->excess_count = g2->connections->size - n2;
    }

    result->excess = (coeff_e * (double)result->excess_count) / (double)max_count;
    result->disjoint_count = disjoint_count;
    result->disjoint = (coeff_d * (double)disjoint_count) / (double)max_count;
    result->avg_weight_diff = coeff_w * (weight_diff / (double)max_count);
    result->delta = result->disjoint + result->excess + result->avg_weight_diff;
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

    Link* conn_clone;
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

Node* get_random_node(Genome* g) {
    int index = rand() % (g->inputs->size + g->hidden->size + g->outputs->size);
    if (index < g->inputs->size) {
        return vector_get(g->inputs, index);
    }
    index = index - g->inputs->size;
    if (index < g->hidden->size) {
        return vector_get(g->hidden, index);
    }
    index = index - g->hidden->size;
    return vector_get(g->outputs, index);
}

int mutate_split_connection(Genome* g) {
    if (g->connections->size == 0) {
        return -1;
    }
    int r = rand() % g->connections->size;
    Link* c = vector_get(g->connections, r);
    while (!c->enabled || (c->out->level - c->in->level) <= 1) {
        r = rand() % g->connections->size;
        c = vector_get(g->connections, r);
    }

    c->enabled = false;

    Node* node = new_node();
    node->level = (rand() % (c->out->level - (c->in->level+1))) + c->in->level + 1;

    Link* c1 = new_connection(1.0, true);
    Link* c2 = new_connection(c->weight, true);
    connect(c1, c->in, node);
    connect(c2, node, c->out);

    add_connection(g, c1);
    add_connection(g, c2);
    vector_append(g->hidden, node);

    return 0;
}

int mutate_connect(Genome* g) {
    Node* node1;
    Node* node2;

    do {
        node1 = get_random_node(g);
        node2 = get_random_node(g);
    } while(node1->level >= node2->level);

    double weight = ((double)rand() / (double)RAND_MAX) * 2.0;
    Link* c = new_connection(weight, true);
    connect(c, node1, node2);
    add_connection(g, c);

    return 0;
}

int mutate_weight(Genome* g) {
    int r = rand() % g->connections->size;
    Link* c = vector_get(g->connections, r);
    c->weight *= ((double)rand() / (double)RAND_MAX);
    return 0;
}

Genome* mate(Genome* g1, Genome* g2) {
    Genome* offspring = new_genome(0, 0);
    Link* c1;
    Link* c2;
    Link* new_c;
    Node* node_in;
    Node* node_out;
    int n1 = 0, n2 = 0;

    offspring->inputs = clone_nodes(g1->inputs);
    offspring->outputs = clone_nodes(g1->outputs);

    while (n1 < g1->connections->size || n2 < g2->connections->size) {
        if (n1 < g1->connections->size && n2 < g2->connections->size) {
            c1 = vector_get(g1->connections, n1);
            c2 = vector_get(g2->connections, n2);
            if (c1->inumber < c2->inumber) {
                // not in c2
                new_c = clone_connection(c1);
                n1++;
            } else if (c2->inumber < c1->inumber) {
                // not in c1
                new_c = clone_connection(c2);
                n2++;
            } else {
                if (g1->fitness < g2->fitness) {
                    new_c = clone_connection(c1);
                } else {
                    new_c = clone_connection(c2);
                }
                n1++;
                n2++;
            }
        } else if (n1 < g1->connections->size) {
            c1 = vector_get(g1->connections, n1);
            new_c = clone_connection(c1);
            n1++;
        } else {
            c2 = vector_get(g2->connections, n2);
            new_c = clone_connection(c2);
            n2++;
        }
        node_in = find_node_in_genome(offspring, new_c->in->id);
        node_out = find_node_in_genome(offspring, new_c->out->id);
        if (node_in == NULL) {
            node_in = clone_node(new_c->in);
            vector_append(offspring->hidden, node_in);
        }
        if (node_out == NULL) {
            node_out = clone_node(new_c->out);
            vector_append(offspring->hidden, node_out);
        }
        new_c->in = NULL;
        new_c->out = NULL;
        add_connection(offspring, new_c);
        connect(new_c, node_in, node_out);
    }

    return offspring;
}

void _reset_nodes(Vector* nodes) {
    int i;
    Node* node;
    for (i=0; i<nodes->size; i++) {
        node = vector_get(nodes, i);
        node->input_count = 0;
        node->value = 0.0;
        node->evaluated = false;
    }
}

void reset_genome(Genome* g) {
    _reset_nodes(g->inputs);
    _reset_nodes(g->hidden);
    _reset_nodes(g->outputs);
}

void set_genome_inputs(Genome* g, Vector* inputs) {
    if (g->inputs->size != inputs->size) {
        fprintf(stderr, "Genome inputs vector and supplied inputs are not of same size.\n");
        exit(-1);
    }
    reset_genome(g);
    int i;
    double* value;
    Node* node;
    for (i=0; i<inputs->size; i++) {
        node = vector_get(g->inputs, i);
        value = vector_get(inputs, i);
        node->value = *value;
        node->input_count = 1;
        node->evaluated = true;
    }
}

void get_genome_outputs(Genome* g, double* outputs) {
    int i;
    Node* node;
    for (i=0; i<g->outputs->size; i++) {
        node = vector_get(g->outputs, i);
        outputs[i] = node->value;
    }
}

void evaluate_genome(Genome* g) {
    int level;
    int i;
    Link* c;
    for (level=0; level<=MAX_HIDDEN_LEVELS+1; level++) {
        for (i=0; i<g->connections->size; i++) {
            c = vector_get(g->connections, i);
            if (!c->enabled) {
                continue;
            }
            if (c->in->level == level) {
                if (!c->in->evaluated) {
                    if (c->in->input_count > 0) {
                        c->in->value = c->in->value / (double)c->in->input_count;
                    } else {
                        c->in->value = 0.0;
                    }
                    c->in->evaluated = true;
                }
                c->out->value += (c->in->value * c->weight);
                c->out->input_count++;
            }
        }
    }

    Node* node;
    for (i=0; i<g->outputs->size; i++) {
        node = vector_get(g->outputs, i);
        if (node->input_count > 0) {
            node->value = node->value / (double)node->input_count;
        } else {
            node->value = 0.0;
        }
    }
}

double calculate_simple_fitness(double* outputs, double* desired, size_t size) {

    double delta[size];
    double deviations[size];
    double mean_delta = 0.0;
    double variance = 0.0;
    double pstdev;
    int i;

    for (i=0; i<size; i++) {
        delta[i] = fabs(outputs[i] - desired[i]);
        mean_delta += delta[i];
    }
    mean_delta /= size;

    for (i=0; i<size; i++) {
        deviations[i] = pow(delta[i] - mean_delta, 2.0);
        variance += deviations[i];
    }
    variance /= size;
    pstdev = sqrt(variance);
    return mean_delta * pstdev;
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
    Link* c;
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
        fprintf(f, "%d [%d]", node->id, node->level);
    }
    fprintf(f, "\n");
}
