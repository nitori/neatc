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

#ifndef NEAT_SPECIES_H
#define NEAT_SPECIES_H

#include "genome.h"

#include "list.h"

typedef struct Genome Genome;

typedef struct Species {
    List genomes;
} Species;


Species* new_species();
void species_init(Species* species);

void species_add_genome(Species* species, Genome* genome);
void species_remove_genome(Species* species, Genome* genome);

void species_free(Species* species);


#endif //NEAT_SPECIES_H
