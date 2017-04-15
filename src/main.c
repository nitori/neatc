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

#include <stdio.h>
#include <sys/time.h>


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
        printf("  Neuron: [%p, id: %d, level: %d, v: %f, i: %d] (%s)\n", neuron, (int)neuron->id, neuron->level, neuron->value, neuron->input_links, type);
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
    struct timeval time;
    gettimeofday(&time, NULL);
    unsigned long long microsec = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
    srand((unsigned int)microsec);

    Population population;
    population_init(&population);

    Innovation* innovation;

    Genome g1;

    int i;
    int max = 100;
    int hidden = 20;

    if (argc == 2) {
        max = atoi(argv[1]);
    } else if (argc == 3) {
        max = atoi(argv[1]);
        hidden = atoi(argv[2]);
    }

    genome_init(&g1, hidden, 5, 1);

    for (i=0; i<max; i++) {
        innovation = new_innovation();
        if (i == 0) {
            genome_mutate_add_link(&g1, &population, innovation);
        } else {
            genome_mutate(&g1, &population, innovation);
        }
    }

    double inputs[g1.input_count];
    double outputs[g1.output_count];

    for (i=0; i<g1.input_count; i++) {
        inputs[i] = 1.0;
    }

    genome_calculate_output(&g1, inputs, outputs);

    genome_dump(&g1);

    fprintf(stderr, "Outputs: ");
    for (i=0; i<g1.output_count; i++) {
        if (i>0) {
            fprintf(stderr, ", ");
        }
        fprintf(stderr, "%f", outputs[i]);
    }
    fprintf(stderr, "\n");

    return 0;
}
