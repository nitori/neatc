//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_INNOVATION_H
#define NEAT_INNOVATION_H

#include "types.h"


typedef enum InnovationType {
    InnovationLinkType,
    InnovationNodeType,
} InnovationType;

typedef struct LinkInnovation {
    NeuronId node_in_id;
    NeuronId node_out_id;
    INumber new_link_inumber;
} LinkInnovation;

typedef struct NodeInnovation {
    INumber inumber;
    NeuronId new_node_id;
    INumber new_link_in_inumber;
    INumber new_link_out_inumber;
} NodeInnovation;

typedef struct Innovation {
    InnovationType type;
    union {
        LinkInnovation link;
        NodeInnovation node;
    } innovation;
} Innovation;


Innovation* new_link_innovation(
        NeuronId node_in_id,
        NeuronId node_out_id,
        INumber new_link_inumber);

Innovation* new_node_innovation(
        INumber inumber,
        NeuronId new_node_id,
        INumber new_link_in_inumber,
        INumber new_link_out_inumber);

INumber next_inumber();

#endif //NEAT_INNOVATION_H
