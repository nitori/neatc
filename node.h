//
// Created by nitori on 16.03.17.
//

#ifndef NEAT_NODE_H
#define NEAT_NODE_H

#define MAX_HIDDEN_LEVELS 10

struct Node {
    int32_t id;
    double value;
    int32_t input_count;
    int32_t level;
    bool evaluated;
};

Node* new_node();
Node* clone_node(Node* node);
void free_node(Node* node);
void free_nodes(Vector* v);

#endif //NEAT_NODE_H
