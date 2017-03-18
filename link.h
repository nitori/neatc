//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_CONNECTION_H
#define NEAT_CONNECTION_H

struct Link {
    Node* in;
    Node* out;
    double weight;
    bool enabled;
    int32_t inumber;  // innovation number
    Genome* owner;
};


Link* new_connection(double weight, bool enabled);
void connect(Link* c, Node* in, Node* out);
int add_connection(Genome* g, Link* c);
Link* clone_connection(Link* c);
Link* find_connection(Genome* g, int32_t inumber);
void free_connection(Link* c);

#endif //NEAT_CONNECTION_H
