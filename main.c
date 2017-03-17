
#include "neat.h"


int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    Genome *g1 = new_genome(5, 5);
    mutate_connect(g1);  // add at least one connection

    int mutation_count = 10;
    if (argc > 1) {
        mutation_count = atoi(argv[1]);
    }

    int i;
    int choice;
    int count_connects, count_splits, count_weight;
    count_connects = count_splits = count_weight = 0;
    for (i=0; i<mutation_count; i++) {
        choice = rand() % 5;
        if (choice == 0) {
            mutate_connect(g1);
            count_connects++;
        } else if (choice == 1) {
            mutate_split_connection(g1);
            count_splits++;
        } else {
            mutate_weight(g1);
            count_weight++;
        }
    }

    save_genome(g1, "genome.txt");

    Vector inputs;
    vector_init(&inputs);

    double a[] = {1.0, 50.0, -30.0, 5.0, 123.0};
    vector_append(&inputs, &a[0]);
    vector_append(&inputs, &a[1]);
    vector_append(&inputs, &a[2]);
    vector_append(&inputs, &a[3]);
    vector_append(&inputs, &a[4]);

    set_genome_inputs(g1, &inputs);
    vector_free(&inputs);

    evaluate_genome(g1);

    printf("Mutation statistics:\n");
    printf("  Total mutations: %d\n", count_connects + count_splits + count_weight);
    printf("    New connections:   %d\n", count_connects);
    printf("    New nodes (split): %d\n", count_splits);
    printf("    Weight changes:    %d\n", count_weight);

    return 0;
}