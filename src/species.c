//
// Created by nitori on 02.04.17.
//

#include "species.h"

#include "genome.h"

#include "list.h"

#include <stdlib.h>


Species* new_species() {
    return calloc(1, sizeof(Species));
}

void species_init(Species* species) {
    list_init(&species->genomes);
}

void species_add_genome(Species* species, Genome* genome) {
    // add to specified species and remove it from other species if necessary
    if (genome->species != species) {
        if (genome->species != NULL) {
            species_remove_genome(genome->species, genome);
        }
        list_append(&species->genomes, new_listitem(genome));
        genome->species = species;
    }
}

void species_remove_genome(Species* species, Genome* genome) {
    int i;
    ListItem* item;
    for (i=0; i<species->genomes.size; i++) {
        item = list_get(&species->genomes, i);
        if (item->data == genome) {
            list_remove(&species->genomes, i);
            return;
        }
    }
}

void species_free(Species* species) {
    list_free(&species->genomes);
}
