#include <stdio.h>
#include <stdlib.h>
#include "neat.h"


int main() {

    Genome *g1 = init_genome(0, 0);
    Genome *g2 = init_genome(0, 0);

    add_connection(g1, init_connection(NULL, NULL, 1.0, true));
    add_connection(g1, init_connection(NULL, NULL, 1.0, true));
    add_connection(g1, init_connection(NULL, NULL, 1.4, true));

    add_connection(g2, clone_connection(g1->head));
    add_connection(g2, clone_connection(g1->head->next));
    g2->head->weight = 0.9;
    g2->head->next->weight = 1.4;

    add_connection(g2, init_connection(NULL, NULL, 1.0, true));
    add_connection(g2, init_connection(NULL, NULL, 2.3, true));

    print_genome(g1);
    print_genome(g2);

    DeltaResult* delta = delta_genomes(g1, g2, 1.0, 1.0, 1.0);

    printf("disjoint count: %d\n", delta->disjoint_count);
    printf("disjoint: %f\n", delta->disjoint);
    printf("excess count: %d\n", delta->excess_count);
    printf("excess: %f\n", delta->excess);
    printf("avg weight diff: %f\n", delta->avg_weight_diff);
    printf("DELTA: %f\n", delta->delta);

    return 0;
}