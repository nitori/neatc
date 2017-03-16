//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_NODE_H
#define NEAT_NODE_H

typedef enum NodeType {
    inputNode,
    hiddenNode,
    outputNode,
} NodeType;

struct Node {
    NodeType type;

    Connection* from;
    Connection* to;

    Node* next;
    Node* prev;

    Genome* owner;
};


Node* init_node();
int add_node(Genome* g, Node* node);
int add_hidden_node(Genome* g, Node* node);
Node* get_node(Genome* g, int32_t index);

#endif //NEAT_NODE_H
