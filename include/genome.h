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

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

#include "species.h"
#include "neuron.h"
#include "link.h"

#include "innovation.h"

#include "list.h"

typedef struct Population Population;
typedef struct Species Species;

typedef struct Genome {
    // reference to species to ensure one genome
    // is always only part of one species
    Species* species;
    double fitness;
    int32_t max_levels;
    size_t input_count;
    size_t output_count;
    List neurons;
    List links;
} Genome;

Genome* new_genome();
void genome_init(Genome* genome, int32_t hidden_count, size_t number_of_inputs, size_t number_of_outputs);
void genome_init_neurons(Genome* genome, size_t number_of_neurons, int32_t level);

int genome_mutate_add_link(Genome* genome, Population* population, Innovation* innovation);
int genome_mutate_add_neuron(Genome* genome, Population* population, Innovation* innovation);
int genome_mutate(Genome* genome, Population* population, Innovation* innovation);
void genome_clone(Genome* genome, Genome* clone);
double genome_diff(Genome* g1, Genome* g2, double c1, double c2, double c3);
void genome_crossover(Genome* parent1, Genome* parent2, Genome* offspring);
void genome_calculate_output(Genome* genome, double inputs[], double outputs[]);

void genome_dump(Genome* genome);

void genome_free(Genome* genome);

#endif //NEAT_GENOME_H
