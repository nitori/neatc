//
// Created by nitori on 02.04.17.
//

#include "population.h"

#include "list.h"

#include <stdlib.h>


Population* new_population() {
    return calloc(1, sizeof(Population));
}

void population_init(Population* population) {
    list_init(&population->innovations);
    list_init(&population->species);
}

void population_add_species(Population* population, Species* species) {
    list_append(&population->species, new_listitem(species));
}

void population_add_innovation(Population* population, Innovation* innovation) {
    list_append(&population->innovations, new_listitem(innovation));
}

void population_free(Population* population) {
    list_free(&population->innovations);
    list_free(&population->species);
}