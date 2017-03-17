//
// Created by nitori on 17.03.17.
//
#include "neat.h"


Species* new_species() {
    Species* species = calloc(1, sizeof(Species));
    species->genomes = new_vector();
    return species;
}

int add_genome(Species* s, Genome* g) {
    if (g->owner != NULL) {
        fprintf(stderr, "Genome is already owned by another species.\n");
        return -1;
    }
    g->owner = s;
    vector_append(s->genomes, g);
    return 0;
}

void free_species(Species* species) {
    vector_free(species->genomes);
    free(species);
}

