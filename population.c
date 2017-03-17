//
// Created by nitori on 17.03.17.
//

#include "neat.h"


Population* new_population() {
    Population* population = calloc(1, sizeof(Population));
    population->species = new_vector();
    return population;
}

int add_species(Population* p, Species* s) {
    vector_append(p->species, s);
    return 0;
}

void free_population(Population* population) {
    vector_free(population->species);
    free(population);
}
