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

#include "neuron.h"
#include "innovation.h"
#include <stdlib.h>

Neuron* new_neuron() {
    return calloc(1, sizeof(Neuron));
}

void neuron_init(Neuron* neuron) {
    neuron->id = next_inumber();
    neuron->type = NormalNeuron;
    neuron->level = 0;
    neuron->value = 0.0;
}

void neuron_free(Neuron* neuron) {

}
