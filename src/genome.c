//
// Created by nitori on 18.03.17.
//

#include "genome.h"

#include <stdlib.h>

Genome* new_genome() {
    return calloc(1, sizeof(Genome));
}

void genome_init(Genome* genome) {
    genome->species = NULL;
}

