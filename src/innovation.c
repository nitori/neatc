//
// Created by nitori on 18.03.17.
//

#include "innovation.h"

#include <stdlib.h>


Innovation* new_link_innovation(
        NeuronId node_in_id,
        NeuronId node_out_id,
        INumber new_link_inumber) {

    Innovation* innov = calloc(1, sizeof(Innovation));
    innov->type = InnovationLinkType;
    innov->innovation.link.new_link_inumber = new_link_inumber;
    innov->innovation.link.node_in_id = node_in_id;
    innov->innovation.link.node_out_id = node_out_id;
    return innov;
}

Innovation* new_node_innovation(
        INumber inumber,
        NeuronId new_node_id,
        int32_t new_node_level,
        INumber new_link_in_inumber,
        INumber new_link_out_inumber) {

    Innovation* innov = calloc(1, sizeof(Innovation));
    innov->type = InnovationNodeType;
    innov->innovation.node.inumber = inumber;
    innov->innovation.node.new_node_level = new_node_level;
    innov->innovation.node.new_node_id = new_node_id;
    innov->innovation.node.new_link_in_inumber = new_link_in_inumber;
    innov->innovation.node.new_link_out_inumber = new_link_out_inumber;
    return innov;
}

INumber next_inumber() {
    static INumber global_innovation_counter = 0;
    global_innovation_counter++;
    return global_innovation_counter;
}
