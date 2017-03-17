//
// Created by nitori on 17.03.17.
//

#ifndef NEAT_SPECIES_H
#define NEAT_SPECIES_H


struct Species {
    Vector* genomes;
    double fitness;
};


Species* new_species();
int add_genome(Species* s, Genome* g);
void free_species(Species* s);

#endif //NEAT_SPECIES_H
