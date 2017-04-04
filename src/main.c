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

#include "neat.h"

#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <innovation.h>


void print_genome(Genome* genome, const char* name) {
    printf("Genome %s\n", name);
    int i;
    char* type;
    Neuron* neuron;
    for (i=0; i<genome->neurons.size; i++) {
        neuron = list_get(&genome->neurons, i)->data;
        if (neuron->level == 0) {
            type = "input";
        } else if (neuron->level == genome->max_levels - 1) {
            type = "output";
        } else {
            type = "hidden";
        }
        printf("  Neuron: [%p, id: %d, level: %d] (%s)\n", neuron, (int)neuron->id, neuron->level, type);
    }
    Link* link;
    char* state;
    for (i=0; i<genome->links.size; i++) {
        link = list_get(&genome->links, i)->data;
        if (link->enabled) {
            state = " ON";
        } else {
            state = "OFF";
        }
        printf("  Link: [%p, %s, inumber: %d, weight: %f, neurons: %d->%d, (%p->%p)]\n",
               link,
               state,
               (int)link->inumber,
               link->weight,
               (int)link->in->id,
               (int)link->out->id,
               link->in,
               link->out
        );
    }
    printf("\n");
}


int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    Population population;
    population_init(&population);

    Innovation* innovation;

    Genome g1;

    genome_init(&g1);
    genome_init_inputs(&g1, 5);
    genome_init_outputs(&g1, 5);

    int i;
    for (i=0; i<100; i++) {
        innovation = new_innovation();
        if (i == 0) {
            genome_mutate_add_link(&g1, &population, innovation);
        } else {
            genome_mutate(&g1, &population, innovation);
        }
    }

    genome_dump(&g1);

    return 0;
}