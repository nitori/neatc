//
// Created by nitori on 02.04.17.
//

#ifndef NEAT_SPECIES_H
#define NEAT_SPECIES_H

#include "list.h"

typedef struct Species {
    List genomes;
} Species;


Species* new_species();
void species_init(Species* species);

void species_free(Species* species);


#endif //NEAT_SPECIES_H
