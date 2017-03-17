//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

struct Genome {
    double fitness;

    Vector* connections;

    // nodes
    Vector* inputs;
    Vector* outputs;
    Vector* hidden;
};


int print_genome(Genome* g);
void print_nodes(Vector* nodes);
Genome* new_genome(int32_t input_nodes, int32_t output_nodes);
void delta_genomes(DeltaResult* r, Genome* g1, Genome* g2, double coeff_d, double coeff_w);
Genome* mate(Genome* g1, Genome* g2);
Genome* clone_genome(Genome* g);

Node* find_node_in_genome(Genome* g, int32_t id);
Node* find_node_in_vector(Vector* v, int32_t id);

int mutate_split_connection(Genome* g);
int mutate_connect(Genome* g);
void free_genome(Genome* g);

#endif //NEAT_GENOME_H
