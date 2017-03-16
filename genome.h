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

    int32_t input_count;
    int32_t output_count;
    int32_t hidden_count;
};


int print_genome(Genome* g);
Genome* init_genome(int32_t input_nodes, int32_t output_nodes);
DeltaResult* delta_genomes(Genome* g1, Genome* g2, double coeff_d, double coeff_e, double coeff_w);
Genome* mate(Genome* g1, Genome* g2);
Genome* clone_genome(Genome* g);

int mutate_split_connection(Genome* g);
int mutate_connect(Genome* g);

#endif //NEAT_GENOME_H
