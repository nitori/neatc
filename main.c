
#include "neat.h"

#define INPUT_COUNT 5
#define OUTPUT_COUNT 5
#define INITIAL_GENOMES 30


int main(int argc, char** argv) {
    int i;

    srand((unsigned int)time(NULL));

    Species* species = new_species();
    Genome* g;
    for (i=0; i<INITIAL_GENOMES; i++) {
        g = new_genome(INPUT_COUNT, OUTPUT_COUNT);
        add_genome(species, g);
    }

    Genome* g1 = vector_get(species->genomes, 0);
    Genome* g2 = vector_get(species->genomes, 1);

    mutate_connect(g2);

    Genome* g3 = mate(g1, g2);

    print_genome(g1);
    print_genome(g2);
    print_genome(g3);

    return 0;
}