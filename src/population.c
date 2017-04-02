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

#include "population.h"

#include "list.h"

#include <stdlib.h>
#include <innovation.h>


Population* new_population() {
    return calloc(1, sizeof(Population));
}

void population_init(Population* population) {
    list_init(&population->innovations);
    list_init(&population->species);
}

void population_add_species(Population* population, Species* species) {
    list_append(&population->species, new_listitem(species));
}

void population_add_innovation(Population* population, Innovation* innovation) {
    list_append(&population->innovations, new_listitem(innovation));
}

LinkInnovation* population_find_link_innovation(Population* population, NeuronId neuron_in_id, NeuronId neuron_out_id) {
    int i;
    ListItem* item;
    Innovation* innovation;
    for (i=0; i<population->innovations.size; i++) {
        item = list_get(&population->innovations, i);
        innovation = item->data;
        if (innovation->type == InnovationLinkType) {
            if (innovation->innovation.link.neuron_in_id == neuron_in_id &&
                    innovation->innovation.link.neuron_out_id == neuron_out_id) {
                return &innovation->innovation.link;
            }
        }
    }
    return NULL;
}

NeuronInnovation* population_find_neuron_innovation(Population* population, INumber inumber) {
    int i;
    ListItem* item;
    Innovation* innovation;
    for (i=0; i<population->innovations.size; i++) {
        item = list_get(&population->innovations, i);
        innovation = item->data;
        if (innovation->type == InnovationNeuronType) {
            if (innovation->innovation.neuron.inumber == inumber) {
                return &innovation->innovation.neuron;
            }
        }
    }
    return NULL;
}

void population_free(Population* population) {
    list_free(&population->innovations);
    list_free(&population->species);
}