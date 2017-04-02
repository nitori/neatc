//
// Created by nitori on 02.04.17.
//

#ifndef NEAT_POPULATION_H
#define NEAT_POPULATION_H

#include "species.h"
#include "innovation.h"

#include "types.h"

#include "list.h"

typedef struct Population {
    List innovations;
    List species;
} Population;


Population* new_population();
void population_init(Population* population);

void population_add_species(Population* population, Species* species);
void population_add_innovation(Population* population, Innovation* innovation);

LinkInnovation* population_find_link_innovation(Population* population, NeuronId node_in_id, NeuronId node_out_id);
NodeInnovation* population_find_node_innovation(Population* population, INumber inumber);

void population_free(Population* population);

#endif //NEAT_POPULATION_H
