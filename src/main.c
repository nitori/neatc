
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
        printf("  Neuron: [id: %d, level: %d] (%s)\n", (int)neuron->id, neuron->level, type);
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
        printf("  Link: [%s, inumber: %d, weight: %f, neurons: %d->%d]\n",
               state,
               (int)link->inumber,
               link->weight,
               (int)link->in->id,
               (int)link->out->id
        );
    }
    printf("\n");
}


int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    Population population;
    population_init(&population);

    Genome g1;
    Genome g2;
    genome_init(&g1);
    genome_init(&g2);

    genome_init_inputs(&g1, 1);
    genome_init_outputs(&g1, 1);

    genome_init_inputs(&g2, 1);
    genome_init_outputs(&g2, 1);

    Neuron *n;
    // pretend it's a clone (no clone function written yet)
    n = list_get(&g2.neurons, 0)->data;
    n->id = 1;
    n = list_get(&g2.neurons, 1)->data;
    n->id = 2;

    printf("* Generate two genomes that have the same structure.\n* One input and one output.\n\n");

    print_genome(&g1, "g1");
    print_genome(&g2, "g2");

    Innovation* innovation;

    innovation = new_innovation();
    innovation_init(innovation);

    int result;

    // DO A RANDOM LINK MUTATION ON g1. Because we only have two nodes, the link will
    // be between those two
    result = genome_mutate_add_link(&g1, &population, innovation);

    if (result) {
        printf("* New innovation.\n");
        printf("* Link with inumber %d was created.\n\n", (int)innovation->innovation.link.new_link_inumber);
        population_add_innovation(&population, innovation);
    } else {
        printf("* No new innovation.\n");
        printf("* Link with inumber %d was reused.\n\n", (int)innovation->innovation.link.new_link_inumber);
    }

    print_genome(&g1, "g1");
    print_genome(&g2, "g2");

    innovation = new_innovation();
    innovation_init(innovation);

    // DO A RANDOM LINK MUTATION ON g2. The same two nodes have to be connected,
    // so it must find the already used innovation and apply the same inumber to
    // the new link in g2, as has been done in g1.
    result = genome_mutate_add_link(&g2, &population, innovation);

    if (result) {
        printf("* New innovation.\n");
        printf("* Link with inumber %d was created.\n\n", (int)innovation->innovation.link.new_link_inumber);
        population_add_innovation(&population, innovation);
    } else {
        printf("* No new innovation.\n");
        printf("* Link with inumber %d was reused.\n\n", (int)innovation->innovation.link.new_link_inumber);
    }

    print_genome(&g1, "g1");
    print_genome(&g2, "g2");

    innovation = new_innovation();
    innovation_init(innovation);
    result = genome_mutate_add_neuron(&g1, &population, innovation);
    if (result) {
        population_add_innovation(&population, innovation);
    }

    innovation = new_innovation();
    innovation_init(innovation);
    result = genome_mutate_add_neuron(&g2, &population, innovation);
    if (result) {
        population_add_innovation(&population, innovation);
    }

    print_genome(&g1, "g1");
    print_genome(&g2, "g2");

    return 0;
}