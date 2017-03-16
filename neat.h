//
// Created by nitori on 14.03.17.
//

#ifndef NEAT_NEAT_H
#define NEAT_NEAT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define true 1
#define false 0

typedef uint8_t bool;

struct Connection;
typedef struct Connection Connection;

struct Genome;
typedef struct Genome Genome;

struct Species;
typedef struct Species Species;

struct Population;
typedef struct Population Population;

struct DeltaResult;
typedef struct DeltaResult DeltaResult;

#include "connection.h"
#include "genome.h"


struct Species {
    Genome** genomes;
    int32_t genome_count;
    double fitness;
};


struct Population {
    Species** species;
    int32_t species_count;
};


struct DeltaResult {
    double delta;
    int32_t disjoint_count;
    double disjoint;
    int32_t excess_count;
    double excess;
    double avg_weight_diff;
};


int32_t next_inumber();

#endif //NEAT_NEAT_H
