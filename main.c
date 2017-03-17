
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

    Genome *g2 = clone_genome(g1);

    mutate_split_connection(g2);
    mutate_split_connection(g2);
    mutate_connect(g2);

    print_genome(g1);
    print_genome(g2);

    Vector* inputs = new_vector();
    double a = 1.0, b = 20, c = -15;
    vector_append(inputs, &a);
    vector_append(inputs, &b);
    vector_append(inputs, &c);

    set_genome_inputs(g2, inputs);
    evaluate_genome(g2);

    print_genome(g2);

    return 0;
}