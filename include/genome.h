//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

#include "species.h"
#include "neuron.h"
#include "link.h"

#include "innovation.h"

#include "list.h"

typedef struct Population Population;
typedef struct Species Species;

typedef struct Genome {
    // reference to species to ensure one genome
    // is always only part of one species
    Species* species;

    int32_t max_levels;
    List neurons;
    List links;
} Genome;

Genome* new_genome();
void genome_init(Genome* genome);

void genome_mutate_add_link(Genome* genome, Population* population, Innovation* innovation);
void genome_mutate_add_neuron(Genome* genome, Population* population, Innovation* innovation);
void genome_mutate(Genome* genome, Population* population, Innovation* innovation);

void genome_free(Genome* genome);

#endif //NEAT_GENOME_H
