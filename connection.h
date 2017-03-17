//
// Created by nitori on 16/03/17.
//

#ifndef NEAT_CONNECTION_H
#define NEAT_CONNECTION_H

struct Connection {
    Node* in;
    Node* out;
    double weight;
    bool enabled;
    int32_t inumber;  // innovation number

    Connection* next;
    Connection* prev;

    Genome* owner;
};


Connection* init_connection(Node* in, Node* out, double weight, bool enabled);
int add_connection(Genome* g, Connection* c);
Connection* clone_connection(Connection* c);
Connection* find_connection(Genome* g, int32_t inumber);
Connection* get_connection(Genome* g, int32_t index);
void free_connection(Connection* c);

#endif //NEAT_CONNECTION_H
