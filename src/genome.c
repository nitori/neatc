//
// Created by nitori on 18.03.17.
//

#include "genome.h"

#include "list.h"
#include "population.h"

#include <stdlib.h>
#include <innovation.h>

Genome* new_genome() {
    return calloc(1, sizeof(Genome));
}

void genome_init(Genome* genome) {
    genome->species = NULL;
    // level 0 = input layer
    // level "max_level - 1" = output layer.
    // level 1 to "max_level - 2" = hidden layers.
    genome->max_levels = 10;  // minimum must be 3
    list_init(&genome->links);
    list_init(&genome->neurons);
}

void genome_init_neurons(Genome* genome, size_t number_of_neurons, int32_t level) {
    int i;
    Neuron* neuron;
    for (i=0; i<number_of_neurons; i++) {
        neuron = new_neuron();
        neuron_init(neuron);
        neuron->level = level;
        list_append(&genome->neurons, new_listitem(neuron));
    }
}

void genome_init_inputs(Genome* genome, size_t number_of_inputs) {
    genome_init_neurons(genome, number_of_inputs, 0);
}

void genome_init_outputs(Genome* genome, size_t number_of_outputs) {
    genome_init_neurons(genome, number_of_outputs, genome->max_levels-1);
}

void genome_mutate_add_link(Genome* genome, Population* population, Innovation* innovation) {
    // chose two random unconnected(?) neurons (on different levels)
    // and add a link to connect them.
    Neuron* neuron_in;
    Neuron* neuron_out;
    int r1, r2;
    do {
        r1 = rand() % (int)genome->neurons.size;
        r2 = rand() % (int)genome->neurons.size;
        neuron_in = list_get(&genome->neurons, r1)->data;
        neuron_out = list_get(&genome->neurons, r2)->data;
    } while(neuron_in->level >= neuron_out->level);

    Link* link = new_link();

    LinkInnovation* link_innovation = population_find_link_innovation(population, neuron_in->id, neuron_out->id);
    if (link_innovation == NULL) {
        link_init(link);
        innovation->type = InnovationLinkType;
        innovation->innovation.link.node_in_id = neuron_in->id;
        innovation->innovation.link.node_out_id = neuron_out->id;
        innovation->innovation.link.new_link_inumber = link->inumber;
    } else {
        innovation->type = InnovationNoneType;
        link->weight = (rand() / (1.0 + RAND_MAX)) * 2.0;
        link->enabled = true;
        link->inumber = link_innovation->new_link_inumber;
    }

    link->in = neuron_in;
    link->out = neuron_out;
    list_append(&genome->links, new_listitem(link));
}

void genome_mutate_add_neuron(Genome* genome, Population* population, Innovation* innovation) {
    // chose a random link and split it to add a new neuron.
    Link* link;
    int r;
    do {
        r = rand() % (int) genome->links.size;
        link = list_get(&genome->links, r)->data;
    } while (link->out->level - link->in->level < 2);  // assuming genome->max_levels is at least 3

    Neuron* neuron = new_neuron();
    Link* in = new_link();
    Link* out = new_link();

    NodeInnovation* node_innovation = population_find_node_innovation(population, link->inumber);
    if (node_innovation == NULL) {
        neuron_init(neuron);
        neuron->level = (rand() % (link->out->level - (link->in->level + 1))) + link->in->level + 1;
        link_init(in);
        link_init(out);
        innovation->type = InnovationNodeType;
        innovation->innovation.node.inumber = link->inumber;
        innovation->innovation.node.new_node_id = neuron->id;
        innovation->innovation.node.new_node_level = neuron->level;
        innovation->innovation.node.new_link_in_inumber = in->inumber;
        innovation->innovation.node.new_link_out_inumber = out->inumber;
    } else {
        innovation->type = InnovationNoneType;
        // use existing innovations values
        neuron->id = node_innovation->new_node_id;
        neuron->level = node_innovation->new_node_level;
        in->inumber = node_innovation->new_link_in_inumber;
        out->inumber = node_innovation->new_link_out_inumber;
    }

    in->weight = 1.0;
    out->weight = link->weight;
    in->in = link->in;
    in->out = neuron;
    out->in = neuron;
    out->out = link->out;
    link_free(link);
    free(link);

    list_append(&genome->neurons, new_listitem(neuron));
    list_append(&genome->links, new_listitem(in));
    list_append(&genome->links, new_listitem(out));
}

void genome_mutate(Genome* genome, Population* population, Innovation* innovation) {
    // link or node mutation
    if (rand() % 2 == 0) {
        genome_mutate_add_link(genome, population, innovation);
    } else {
        genome_mutate_add_neuron(genome, population, innovation);
    }
}


void genome_free(Genome* genome) {
    // links and neurons are created within the genome functions
    // so they all have to be freed here.
    int i;
    ListItem* item;
    for (i=0; i<genome->links.size; i++) {
        item = list_get(&genome->links, i);
        link_free(item->data);
        free(item->data);
    }
    for (i=0; i<genome->neurons.size; i++) {
        item = list_get(&genome->neurons, i);
        neuron_free(item->data);
        free(item->data);
    }
    list_free(&genome->links);
    list_free(&genome->neurons);
}
