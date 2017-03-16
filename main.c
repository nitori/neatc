
#include "neat.h"


int main() {
    srand((unsigned int)time(NULL));

    Genome *g1 = init_genome(3, 1);

    print_genome(g1);

    add_connection(g1, init_connection(1, 3, rand() / (double)RAND_MAX, true));
    add_connection(g1, init_connection(2, 3, rand() / (double)RAND_MAX, true));
    add_connection(g1, init_connection(0, 3, rand() / (double)RAND_MAX, true));

    print_genome(g1);

    mutate_connect(g1);

    print_genome(g1);

    mutate_split_connection(g1);
    mutate_split_connection(g1);
    mutate_split_connection(g1);

    print_genome(g1);

    return 0;
}