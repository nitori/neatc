//
// Created by nitori on 17.03.17.
//

#ifndef NEAT_POPULATION_H
#define NEAT_POPULATION_H

struct Population {
    Vector* genomes;
    Vector* species;
};


Population* new_population();
int add_species(Population* p, Species* s);
int add_genome(Population* p, Genome* g);
void free_population(Population* population);

#endif //NEAT_POPULATION_H
