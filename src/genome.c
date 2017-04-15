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

#include "genome.h"

#include "list.h"
#include "population.h"

#include <stdlib.h>
#include <stdio.h>
#include <innovation.h>
#include <string.h>
#include <math.h>

Genome* new_genome() {
    return calloc(1, sizeof(Genome));
}

void genome_init(Genome* genome, int32_t hidden_count, size_t number_of_inputs, size_t number_of_outputs) {
    genome->species = NULL;
    // level 0 = input layer
    // level "max_level - 1" = output layer.
    // level 1 to "max_level - 2" = hidden layers.
    genome->max_levels = hidden_count + 2;  // minimum must be 3
    genome->fitness = 0.0;
    list_init(&genome->links);
    list_init(&genome->neurons);

    genome_init_neurons(genome, number_of_inputs, 0);
    genome_init_neurons(genome, number_of_outputs, genome->max_levels-1);
    genome->input_count = number_of_inputs;
    genome->output_count = number_of_outputs;
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

int genome_mutate_add_link(Genome* genome, Population* population, Innovation* innovation) {
    // chose two random unconnected(?) neurons (on different levels)
    // and add a link to connect them.
    Neuron* neuron_in;
    Neuron* neuron_out;
    int r1, r2;
    int result;
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
        innovation->innovation.link.neuron_in_id = neuron_in->id;
        innovation->innovation.link.neuron_out_id = neuron_out->id;
        result = 1;
    } else {
        link->weight = (rand() / (1.0 + RAND_MAX)) * 2.0;
        link->enabled = true;
        link->inumber = link_innovation->new_link_inumber;
        innovation->innovation.link.neuron_in_id = link_innovation->neuron_in_id;
        innovation->innovation.link.neuron_out_id = link_innovation->neuron_out_id;
        result = 0;
    }

    innovation->type = InnovationLinkType;
    innovation->innovation.link.new_link_inumber = link->inumber;

    link->in = neuron_in;
    link->out = neuron_out;
    list_append(&genome->links, new_listitem(link));
    return result;
}

int genome_mutate_add_neuron(Genome* genome, Population* population, Innovation* innovation) {
    // chose a random link and split it to add a new neuron.
    Link* link;
    int r;
    int result;

    do {
        r = rand() % (int) genome->links.size;
        link = list_get(&genome->links, r)->data;
    } while (link->out->level - link->in->level < 2);  // assuming genome->max_levels is at least 3

    Neuron* neuron = new_neuron();
    Link* in = new_link();
    Link* out = new_link();

    NeuronInnovation* neuron_innovation = population_find_neuron_innovation(population, link->inumber);
    if (neuron_innovation == NULL) {
        neuron_init(neuron);
        neuron->level = (rand() % (link->out->level - (link->in->level + 1))) + link->in->level + 1;
        link_init(in);
        link_init(out);
        innovation->innovation.neuron.inumber = link->inumber;
        result = 1;
    } else {
        // use existing innovations values
        neuron->id = neuron_innovation->new_neuron_id;
        neuron->level = neuron_innovation->new_neuron_level;
        in->inumber = neuron_innovation->new_link_in_inumber;
        out->inumber = neuron_innovation->new_link_out_inumber;
        innovation->innovation.neuron.inumber = neuron_innovation->inumber;
        result = 0;
    }

    innovation->type = InnovationNeuronType;
    innovation->innovation.neuron.new_neuron_id = neuron->id;
    innovation->innovation.neuron.new_neuron_level = neuron->level;
    innovation->innovation.neuron.new_link_in_inumber = in->inumber;
    innovation->innovation.neuron.new_link_out_inumber = out->inumber;

    in->weight = 1.0;
    out->weight = link->weight;
    in->in = link->in;
    in->out = neuron;
    in->enabled = true;
    out->in = neuron;
    out->out = link->out;
    out->enabled = true;
    link->enabled = false;

    list_append(&genome->neurons, new_listitem(neuron));
    list_append(&genome->links, new_listitem(in));
    list_append(&genome->links, new_listitem(out));
    return result;
}

int genome_mutate(Genome* genome, Population* population, Innovation* innovation) {
    // link or neuron mutation
    if (rand() % 2 == 0) {
        return genome_mutate_add_link(genome, population, innovation);
    } else {
        return genome_mutate_add_neuron(genome, population, innovation);
    }
}

void genome_clone(Genome* genome, Genome* clone) {
    // TODO: because genome_init has changed, this probably needs to change as well
    int i, j;
    Neuron* map[genome->neurons.size][2];
    Neuron* neuron;
    Neuron* neuron_clone;

    genome_init(clone, genome->max_levels-2, genome->input_count, genome->output_count);
    clone->max_levels = genome->max_levels;
    clone->species = genome->species;

    for (i=0; i<genome->neurons.size; i++) {
        neuron = list_get(&genome->neurons, i)->data;
        neuron_clone = new_neuron();
        memcpy(neuron_clone, neuron, sizeof(*neuron));
        map[i][0] = neuron;
        map[i][1] = neuron_clone;
        list_append(&clone->neurons, new_listitem(neuron_clone));
    }

    Link* link;
    Link* link_clone;
    for (i=0; i<genome->links.size; i++) {
        link = list_get(&genome->links, i)->data;
        link_clone = new_link();
        memcpy(link_clone, link, sizeof(*link));
        for (j=0; j<genome->neurons.size; j++) {
            if (link_clone->in == map[j][0]) {
                link_clone->in = map[j][1];
            }
            if (link_clone->out == map[j][0]) {
                link_clone->out = map[j][0];
            }
        }
        list_append(&clone->links, new_listitem(link_clone));
    }

}

// this function assumes, links in genomes are ordered ascending by their inumber.
double genome_diff(Genome* g1, Genome* g2, double c1, double c2, double c3) {
    int32_t disjoint_nodes = 0;
    int32_t excess_nodes = 0;
    double acc_weight_diff = 0.0;
    int32_t joint_nodes = 0;
    double N;

    size_t max_size = g1->links.size >= g2->links.size ? g1->links.size : g2->links.size;
    size_t min_size = g1->links.size <= g2->links.size ? g1->links.size : g2->links.size;

    Link* link1;
    Link* link2;
    int i1, i2;

    i1 = i2 = 0;
    while (i1 < min_size && i2 < min_size) {
        link1 = list_get(&g1->links, i1)->data;
        link2 = list_get(&g2->links, i2)->data;
        if (link2->inumber < link1->inumber) {
            disjoint_nodes++;
            i2++;
        } else if(link1->inumber < link2->inumber) {
            disjoint_nodes++;
            i1++;
        } else {
            acc_weight_diff += fabs(link1->weight - link2->weight);
            joint_nodes++;
            i1++;
            i2++;
        }
    }

    if (i1 >= min_size) {
        excess_nodes = (int32_t)max_size - i2;
    } else {
        excess_nodes = (int32_t)max_size - i1;
    }

    N = (max_size > 20 ? max_size : 1);
    return (c1 * excess_nodes) / N
           + (c2 * disjoint_nodes) / N
           + c3 * (acc_weight_diff / (double)joint_nodes);
}

Link* genome_find_link(Genome* genome, INumber inumber) {
    int i;
    Link* link;
    for (i=0; i<genome->links.size; i++) {
        link = list_get(&genome->links, i)->data;
        if (link->inumber == inumber) {
            return link;
        }
    }
    return NULL;
}

Neuron* genome_find_neuron(Genome* genome, NeuronId id) {
    int i;
    Neuron* neuron;
    for (i=0; i<genome->neurons.size; i++) {
        neuron = list_get(&genome->neurons, i)->data;
        if (neuron->id == id) {
            return neuron;
        }
    }
    return NULL;
}


void genome_crossover(Genome* parent1, Genome* parent2, Genome* offspring) {
    // TODO: because genome_init changed, this probably needs to change as well
    // matching links are inherited randomly.
    // disjoint genes are inherited from the more fit gene.
    if (parent1->max_levels != parent2->max_levels) {
        return;
    }
    if (parent1->species != parent2->species) {
        return;
    }

    Link* link1;
    Link* link2;
    Link* link;
    Link* offspring_link;

    link1 = list_get(&parent1->links, (int)parent1->links.size-1)->data;
    link2 = list_get(&parent2->links, (int)parent2->links.size-1)->data;
    INumber max_inumber = max(link1->inumber, link2->inumber);

    link1 = list_get(&parent1->links, 0)->data;
    link2 = list_get(&parent2->links, 0)->data;
    INumber min_inumber = min(link1->inumber, link2->inumber);

    int i;

    genome_init(offspring, parent1->max_levels-2, parent1->input_count, parent1->output_count);
    offspring->species = parent1->species;
    offspring->max_levels = parent1->max_levels;

    for (i=min_inumber; i<=max_inumber; i++) {
        link1 = genome_find_link(parent1, i);
        link2 = genome_find_link(parent2, i);
        link = NULL;
        if (link1 == NULL && link2 == NULL) {
            continue;
        }
        if (link1 != NULL && link2 != NULL) {
            if (rand() % 2 == 0) {
                link = link1;
            } else {
                link = link2;
            }
        } else if (link1 != NULL) {
            if (parent1->fitness > parent2->fitness) {
                link = link1;
            } else if (parent1->fitness == parent2->fitness && rand() % 2 == 0) {
                link = link1;
            }
        } else {
            if (parent2->fitness > parent1->fitness) {
                link = link2;
            } else if (parent2->fitness == parent1->fitness && rand() % 2 == 0) {
                link = link2;
            }
        }
        if (link != NULL) {
            offspring_link = new_link();
            offspring_link->inumber = link->inumber;
            offspring_link->weight = link->weight;
            offspring_link->enabled = link->enabled;
            offspring_link->in = link->in; // CLONE LATER
            offspring_link->out = link->out; // CLONE LATER
            list_append(&offspring->links, new_listitem(offspring_link));
        }
    }

    // neurons, based on links copied
    Neuron* neuron1;
    Neuron* neuron2;
    Neuron* neuron;
    Neuron* offspring_neuron;

    neuron1 = list_get(&parent1->neurons, (int)parent1->neurons.size-1)->data;
    neuron2 = list_get(&parent2->neurons, (int)parent2->neurons.size-1)->data;
    NeuronId max_id = max(neuron1->id, neuron2->id);

    neuron1 = list_get(&parent1->neurons, 0)->data;
    neuron2 = list_get(&parent2->neurons, 0)->data;
    NeuronId min_id = min(neuron1->id, neuron2->id);

    int j;

    for (i=min_id; i<=max_id; i++) {
        neuron1 = genome_find_neuron(parent1, i);
        neuron2 = genome_find_neuron(parent2, i);
        if (neuron1 != NULL) {
            neuron = neuron1;
        } else if (neuron2 != NULL) {
            neuron = neuron2;
        } else {
            continue;
        }
        offspring_neuron = new_neuron();
        offspring_neuron->id = neuron->id;
        offspring_neuron->type = neuron->type;
        offspring_neuron->level = neuron->level;
        offspring_neuron->value = 0.0;
        list_append(&offspring->neurons, new_listitem(offspring_neuron));

        for (j=0; j<offspring->links.size; j++) {
            link = list_get(&offspring->links, j)->data;
            if (link->in->id == offspring_neuron->id) {
                link->in = offspring_neuron;
            }
            if (link->out->id == offspring_neuron->id) {
                link->out = offspring_neuron;
            }
        }
    }
}

void genome_dump(Genome* genome) {
    int i;
    Neuron* neuron;
    ListItem* item;

    printf("inputs = ");
    for (i=0; i<genome->neurons.size; i++) {
        item = list_get(&genome->neurons, i);
        neuron = item->data;
        if (i < genome->input_count) {
            printf("%d [%d], ", neuron->id, neuron->level);
        } else if ((i - genome->input_count) < genome->output_count) {
            if (i == genome->input_count) {
                printf("\noutputs = ");
            }
            printf("%d [%d], ", neuron->id, neuron->level);
        } else {
            if (i == genome->input_count + genome->output_count) {
                printf("\nhidden = ");
            }
            printf("%d [%d], ", neuron->id, neuron->level);
        }
    }
    printf("\n");

    char* state[2] = {"off", "on"};
    Link* link;
    // print links
    printf("links:\n");
    for (i=0; i<genome->links.size; i++) {
        link = list_get(&genome->links, i)->data;
        printf("%d %s %f %d %d\n", link->inumber, state[link->enabled], link->weight, link->in->id, link->out->id);
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
