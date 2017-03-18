//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_INNOVATION_H
#define NEAT_INNOVATION_H



enum InnovationType {
    InnovationLinkType,
    InnovationNodeType,
};

struct LinkInnovation {
    int32_t node_in_id;
    int32_t node_out_id;
    int32_t new_link_inumber;
};
struct NodeInnovation {
    int32_t inumber;
    int32_t new_node_id;
    int32_t new_link_in_inumber;
    int32_t new_link_out_inumber;
};

struct Innovation {
    InnovationType type;
    union {
        LinkInnovation link;
        NodeInnovation node;
    } innovation;
};

Innovation* new_link_innovation(int32_t node_in_id, int32_t node_out_id, int32_t new_link_inumber);
Innovation* new_node_innovation(int32_t inmuber, int32_t new_node_id, int32_t new_link_in_inumber, int32_t new_link_out_inumber);

#endif //NEAT_INNOVATION_H
