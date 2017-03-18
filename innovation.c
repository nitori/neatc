//
// Created by nitori on 18.03.17.
//
#include "neat.h"
#include "innovation.h"


Innovation* new_link_innovation(int32_t node_in_id, int32_t node_out_id, int32_t new_link_inumber) {
    Innovation* innov = calloc(1, sizeof(Innovation));
    innov->type = InnovationLinkType;
    innov->innovation.link.new_link_inumber = new_link_inumber;
    innov->innovation.link.node_in_id = node_in_id;
    innov->innovation.link.node_out_id = node_out_id;
    return innov;
}

Innovation* new_node_innovation(int32_t inumber, int32_t new_node_id,
                                int32_t new_link_in_inumber, int32_t new_link_out_inumber) {
    Innovation* innov = calloc(1, sizeof(Innovation));
    innov->type = InnovationNodeType;
    innov->innovation.node.inumber = inumber;
    innov->innovation.node.new_node_id = new_node_id;
    innov->innovation.node.new_link_in_inumber = new_link_in_inumber;
    innov->innovation.node.new_link_out_inumber = new_link_out_inumber;
    return innov;
}
