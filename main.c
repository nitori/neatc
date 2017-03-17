
#include "neat.h"


int main() {
    srand((unsigned int)time(NULL));

    Genome *g1 = new_genome(3, 1);

    mutate_connect(g1);
    mutate_split_connection(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_split_connection(g1);
    mutate_split_connection(g1);
    mutate_split_connection(g1);
    mutate_split_connection(g1);
    mutate_split_connection(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_connect(g1);
    mutate_connect(g1);

    save_genome(g1, "genome.txt");

    return 0;
}