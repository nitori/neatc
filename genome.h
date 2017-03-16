//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

struct Genome {
    Connection* head;
    Connection* tail;
    int32_t conn_count;
    double fitness;

    Node* node_head;
    Node* node_tail;
    int32_t node_count;
};


int print_genome(Genome* g);
Genome* init_genome(int32_t input_nodes, int32_t output_nodes);
DeltaResult* delta_genomes(Genome* g1, Genome* g2, double coeff_d, double coeff_e, double coeff_w);

#endif //NEAT_GENOME_H
