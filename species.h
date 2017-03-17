//
// Created by nitori on 17.03.17.
//

#ifndef NEAT_SPECIES_H
#define NEAT_SPECIES_H


struct Species {
    Genome* base_genome;
};


Species* new_species();
void free_species(Species* s);

#endif //NEAT_SPECIES_H
