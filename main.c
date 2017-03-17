
#include "neat.h"

#define INPUT_COUNT 5
#define OUTPUT_COUNT 5
#define INITIAL_GENOMES 30


int main(int argc, char** argv) {
    int i;

    srand((unsigned int)time(NULL));

    Population* pop = new_population();
    Species* species = new_species();
    add_species(pop, species);

    Genome* base_genome = new_genome(INPUT_COUNT, OUTPUT_COUNT);
    Genome* clone;
    for (i=0; i<INITIAL_GENOMES; i++) {
        clone = clone_genome(base_genome);
        add_genome(species, clone);
    }

    run_generation(pop);

    return 0;
}