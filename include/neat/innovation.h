//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_INNOVATION_H
#define NEAT_INNOVATION_H

#include <stdint.h>
#include <stdlib.h>

#include "types.h"

typedef enum InnovationType InnovationType;
typedef struct LinkInnovation LinkInnovation;
typedef struct NodeInnovation NodeInnovation;
typedef struct Innovation Innovation;

enum InnovationType {
    InnovationLinkType,
    InnovationNodeType,
};

struct LinkInnovation {
    NodeId node_in_id;
    NodeId node_out_id;
    INumber new_link_inumber;
};
struct NodeInnovation {
    INumber inumber;
    NodeId new_node_id;
    INumber new_link_in_inumber;
    INumber new_link_out_inumber;
};

struct Innovation {
    InnovationType type;
    union {
        LinkInnovation link;
        NodeInnovation node;
    } innovation;
};


Innovation* new_link_innovation(
        NodeId node_in_id,
        NodeId node_out_id,
        INumber new_link_inumber);

Innovation* new_node_innovation(
        INumber inumber,
        NodeId new_node_id,
        INumber new_link_in_inumber,
        INumber new_link_out_inumber);

INumber next_inumber();

#endif //NEAT_INNOVATION_H
