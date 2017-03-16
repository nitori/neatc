
#include "neat.h"


int main() {

    Genome *g1 = init_genome(3, 1);

    add_hidden_node(g1, init_node());

    print_genome(g1);

    add_connection(g1, init_connection(get_node(g1, 0), get_node(g1, 3), 1.0, true));
    add_connection(g1, init_connection(get_node(g1, 1), get_node(g1, 3), 1.0, true));
    add_connection(g1, init_connection(get_node(g1, 2), get_node(g1, 4), 1.4, true));

    print_genome(g1);


    return 0;
}