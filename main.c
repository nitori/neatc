
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

    return 0;
}