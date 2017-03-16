
#include "neat.h"


int main() {
    srand((unsigned int)time(NULL));

    Genome *g1 = init_genome(3, 1);

    print_genome(g1);

    mutate_connect(g1);
    mutate_split_connection(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_split_connection(g1);

    Genome *g2 = clone_genome(g1);

    print_genome(g1);
    print_genome(g2);

    printf("----------\n");

    mutate_connect(g2);
    mutate_connect(g2);
    mutate_split_connection(g2);

    print_genome(g1);
    print_genome(g2);

    Genome* g3 = mate(g2, g1);

    print_genome(g3);

    return 0;
}