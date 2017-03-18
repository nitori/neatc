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

#include "vector.h"
#include "list.h"

typedef struct Node Node;
typedef struct Link Link;
typedef struct Genome Genome;
typedef struct Species Species;
typedef struct Population Population;
typedef struct DeltaResult DeltaResult;

#include "node.h"
#include "link.h"
#include "genome.h"
#include "species.h"
#include "population.h"


struct DeltaResult {
    double delta;
    int32_t disjoint_count;
    double disjoint;
    int32_t excess_count;
    double excess;
    double avg_weight_diff;
};


int32_t next_inumber();
void run_generation(Population* pop);

#endif //NEAT_NEAT_H
