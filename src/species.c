//
// Created by nitori on 02.04.17.
//

#include "species.h"

#include "list.h"

#include <stdlib.h>


Species* new_species() {
    return calloc(1, sizeof(Species));
}

void species_init(Species* species) {
    list_init(&species->genomes);
}

void species_free(Species* species) {
    list_free(&species->genomes);
}
