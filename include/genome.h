//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

#include "species.h"

#include "types.h"

typedef struct Species Species;

typedef struct Genome {
    // reference to species to ensure one genome
    // is always only part of one species
    Species* species;
} Genome;

Genome* new_genome();
void genome_init(Genome* genome);


#endif //NEAT_GENOME_H
