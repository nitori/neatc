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

#ifndef NEAT_NEURON_H
#define NEAT_NEURON_H

#include "list.h"
#include "types.h"

typedef enum NeuronType {
    NormalNeuron,
    BiasNeuron,
} NeuronType;

typedef struct Neuron {
    NeuronId id;
    NeuronType type;
    int32_t level;
    double value;
    int32_t input_links;
} Neuron;

Neuron* new_neuron();
void neuron_init(Neuron* neuron);

void neuron_free(Neuron* neuron);

#endif //NEAT_NEURON_H
