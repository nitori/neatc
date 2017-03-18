//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_TYPES_H
#define NEAT_TYPES_H

#include <stdint.h>

#include "list.h"

#define true 1
#define false 0

typedef int64_t INumber;
typedef int64_t NeuronId;
typedef int bool;

// genome.h
typedef struct Genome Genome;

// innovation.h
typedef enum InnovationType InnovationType;
typedef struct LinkInnovation LinkInnovation;
typedef struct NodeInnovation NodeInnovation;
typedef struct Innovation Innovation;

// neuron.h
typedef enum NeuronType NeuronType;
typedef struct Neuron Neuron;

// link.h
typedef struct Link Link;

#endif //NEAT_TYPES_H
