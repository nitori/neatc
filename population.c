//
// Created by nitori on 17.03.17.
//

#include "neat.h"


Population* new_population() {
    Population* population = calloc(1, sizeof(Population));
    population->species = new_vector();
    population->genomes = new_vector();
    return population;
}

int add_species(Population* p, Species* s) {
    vector_append(p->species, s);
    return 0;
}

int add_genome(Population* p, Genome* g) {
    vector_append(p->genomes, g);
    return 0;
}


void free_population(Population* population) {
    vector_free(population->species);
    vector_free(population->genomes);
    free(population);
}
