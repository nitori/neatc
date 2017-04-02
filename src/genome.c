//
// Created by nitori on 18.03.17.
//

#include "genome.h"

#include "list.h"
#include "population.h"

#include <stdlib.h>
#include <innovation.h>
#include <string.h>
#include <math.h>

Genome* new_genome() {
    return calloc(1, sizeof(Genome));
}

void genome_init(Genome* genome) {
    genome->species = NULL;
    // level 0 = input layer
    // level "max_level - 1" = output layer.
    // level 1 to "max_level - 2" = hidden layers.
    genome->max_levels = 10;  // minimum must be 3
    genome->fitness = 0.0;
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
    int i, j;
    Neuron* map[genome->neurons.size][2];
    Neuron* neuron;
    Neuron* neuron_clone;

    genome_init(clone);
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
    // matching links are inherited randomly.
    // disjoint genes are inherited from the more fit gene.
    if (parent1->max_levels != parent2->max_levels) {
        return;
    }
    if (parent1->species != parent2->species) {
        return;
    }

    size_t max_size = (size_t)fmax(parent1->links.size, parent2->links.size);
    size_t min_size = (size_t)fmin(parent1->links.size, parent2->links.size);
    Link* link1;
    Link* link2;
    Link* link;
    Link* offspring_link;

    int choice;
    int i1, i2;

    genome_init(offspring);
    offspring->species = parent1->species;
    offspring->max_levels = parent1->max_levels;

    // links
    i1 = i2 = 0;
    while (i1 < min_size && i2 < min_size) {
        link1 = list_get(&parent1->links, i1)->data;
        link2 = list_get(&parent2->links, i2)->data;
        if (link1->inumber < link2->inumber) {
            if (parent2->fitness > parent1->fitness) {
                choice = 0;
            } else if (parent2->fitness < parent1->fitness) {
                choice = 1;
            } else {
                choice = rand() % 2;
            }
            if (choice == 0) {
                link = link2;
            } else {
                link = NULL;
            }
            i1++;
        } else if (link2->inumber < link1->inumber) {
            if (parent2->fitness > parent1->fitness) {
                choice = 0;
            } else if (parent2->fitness < parent1->fitness) {
                choice = 1;
            } else {
                choice = rand() % 2;
            }
            if (choice == 0) {
                link = link2;
            } else {
                link = NULL;
            }
            i2++;
        } else {
            choice = rand() % 2;
            if (choice == 0) {
                link = link1;
            }else {
                link = link2;
            }
            i1++;
            i2++;
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

    // also test excess links
    int i;
    Genome* parent = NULL;

    if (i1 >= min_size) {
        if (parent2->fitness == parent1->fitness && rand() % 2 == 0 ||
                parent2->fitness > parent1->fitness) {
            parent = parent2;
        }
    } else {
        if (parent1->fitness == parent2->fitness && rand() % 2 == 0 ||
                parent1->fitness > parent2->fitness) {
            parent = parent1;
        }
    }

    if (parent != NULL) {
        for (i = (int) min_size; i < max_size; i++) {
            link = list_get(&parent->links, i)->data;
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
    NeuronId max_id = (NeuronId)fmax(
            ((Neuron*)(list_get(&parent1->neurons, (int)parent1->neurons.size-1)->data))->id,
            ((Neuron*)(list_get(&parent2->neurons, (int)parent2->neurons.size-1)->data))->id
    );
    NeuronId min_id = (NeuronId)fmax(
            ((Neuron*)(list_get(&parent1->neurons, 0)->data))->id,
            ((Neuron*)(list_get(&parent2->neurons, 0)->data))->id
    );
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
