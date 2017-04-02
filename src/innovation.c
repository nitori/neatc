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

#include "innovation.h"

#include <string.h>
#include <stdlib.h>

Innovation* new_innovation() {
    return calloc(1, sizeof(Innovation));
}

void innovation_init(Innovation* innovation) {
    memset(innovation, 0, sizeof(*innovation));
}

Innovation* new_link_innovation(
        NeuronId neuron_in_id,
        NeuronId neuron_out_id,
        INumber new_link_inumber) {

    Innovation* innov = calloc(1, sizeof(Innovation));
    innov->type = InnovationLinkType;
    innov->innovation.link.new_link_inumber = new_link_inumber;
    innov->innovation.link.neuron_in_id = neuron_in_id;
    innov->innovation.link.neuron_out_id = neuron_out_id;
    return innov;
}

Innovation* new_neuron_innovation(
        INumber inumber,
        NeuronId new_neuron_id,
        int32_t new_neuron_level,
        INumber new_link_in_inumber,
        INumber new_link_out_inumber) {

    Innovation* innov = calloc(1, sizeof(Innovation));
    innov->type = InnovationNeuronType;
    innov->innovation.neuron.inumber = inumber;
    innov->innovation.neuron.new_neuron_level = new_neuron_level;
    innov->innovation.neuron.new_neuron_id = new_neuron_id;
    innov->innovation.neuron.new_link_in_inumber = new_link_in_inumber;
    innov->innovation.neuron.new_link_out_inumber = new_link_out_inumber;
    return innov;
}

INumber next_inumber() {
    static INumber global_innovation_counter = 0;
    global_innovation_counter++;
    return global_innovation_counter;
}
