//
// Created by nitori on 14.03.17.
//

#ifndef NEAT_NEAT_H
#define NEAT_NEAT_H

#include <stdint.h>
#define true 1
#define false 0

typedef uint8_t bool;

typedef struct NodeGene {

} NodeGene;

typedef struct ConnectionGene {
    NodeGene* in;
    NodeGene* out;
    double weight;
    bool enabled;
    int32_t inumber;  // innovation number

    struct Genome* owner;

    struct ConnectionGene* next;
    struct ConnectionGene* prev;
} ConnectionGene;


typedef struct Genome {
    ConnectionGene* head;
    ConnectionGene* tail;
    int32_t conn_count;
    double fitness;
} Genome;


typedef struct Species {
    Genome** genomes;
    int32_t genome_count;
    double fitness;
} Species;


typedef struct Population {
    Species** species;
    int32_t species_count;
} Population;


typedef struct DeltaResult {
    double delta;
    int32_t disjoint_count;
    double disjoint;
    int32_t excess_count;
    double excess;
    double avg_weight_diff;
} DeltaResult;


int32_t next_inumber();
int print_genome(Genome* g);

Genome* init_genome(int32_t input_nodes, int32_t output_nodes);
ConnectionGene* init_connection(NodeGene* in, NodeGene* out, double weight, bool enabled);
int add_connection(Genome* g, ConnectionGene* c);
ConnectionGene* clone_connection(ConnectionGene* c);
int add_node(Genome* g, ConnectionGene* c_old);


DeltaResult* delta_genomes(Genome* g1, Genome* g2, double coeff_d, double coeff_e, double coeff_w);

#endif //NEAT_NEAT_H
