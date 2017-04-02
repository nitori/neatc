/*
 * NEATC. C implementation of NEAT: NeuroEvolution of Augmented Topologies.
 * Copyright (C) 2017  Lars P. Søndergaard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
