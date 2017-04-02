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

#ifndef NEAT_POPULATION_H
#define NEAT_POPULATION_H

#include "species.h"
#include "innovation.h"

#include "types.h"

#include "list.h"

typedef struct Population {
    List innovations;
    List species;
} Population;


Population* new_population();
void population_init(Population* population);

void population_add_species(Population* population, Species* species);
void population_add_innovation(Population* population, Innovation* innovation);

LinkInnovation* population_find_link_innovation(Population* population, NeuronId neuron_in_id, NeuronId neuron_out_id);
NeuronInnovation* population_find_neuron_innovation(Population* population, INumber inumber);

void population_free(Population* population);

#endif //NEAT_POPULATION_H
