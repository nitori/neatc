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

#ifndef NEAT_INNOVATION_H
#define NEAT_INNOVATION_H

#include "types.h"


typedef enum InnovationType {
    InnovationLinkType,
    InnovationNeuronType,
} InnovationType;

typedef struct LinkInnovation {
    NeuronId neuron_in_id;
    NeuronId neuron_out_id;
    INumber new_link_inumber;
} LinkInnovation;

typedef struct NeuronInnovation {
    INumber inumber;
    NeuronId new_neuron_id;
    int32_t new_neuron_level;
    INumber new_link_in_inumber;
    INumber new_link_out_inumber;
} NeuronInnovation;

typedef struct Innovation {
    InnovationType type;
    union {
        LinkInnovation link;
        NeuronInnovation neuron;
    } innovation;
} Innovation;


Innovation* new_innovation();
void innovation_init(Innovation* innovation);

Innovation* new_link_innovation(
        NeuronId neuron_in_id,
        NeuronId neuron_out_id,
        INumber new_link_inumber);

Innovation* new_neuron_innovation(
        INumber inumber,
        NeuronId new_neuron_id,
        int32_t new_neuron_level,
        INumber new_link_in_inumber,
        INumber new_link_out_inumber);

INumber next_inumber();

#endif //NEAT_INNOVATION_H
