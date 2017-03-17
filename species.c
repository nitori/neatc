//
// Created by nitori on 17.03.17.
//
#include "neat.h"


Species* new_species() {
    Species* species = calloc(1, sizeof(Species));
    species->base_genome = NULL;
    return species;
}

void free_species(Species* species) {
    free(species);
}

