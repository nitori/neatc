
#include "neat.h"


int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    Genome *g1 = new_genome(3, 1);
    mutate_connect(g1);  // add at least one connection

    int mutation_count = 10;
    if (argc > 1) {
        mutation_count = atoi(argv[1]);
    }

    int i;
    int choice;
    for (i=0; i<mutation_count; i++) {
        choice = rand() % 2;
        if (choice == 0) {
            mutate_connect(g1);
        } else {
            mutate_split_connection(g1);
        }
    }

    save_genome(g1, "genome.txt");

    Vector* inputs = new_vector();
    double a = 1.0;
    double b = 50.0;
    double c = -30.0;
    vector_append(inputs, &a);
    vector_append(inputs, &b);
    vector_append(inputs, &c);

    set_genome_inputs(g1, inputs);
    evaluate_genome(g1);

    print_genome(g1);

    return 0;
}