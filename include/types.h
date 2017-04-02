//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_TYPES_H
#define NEAT_TYPES_H

#include <stdint.h>

#define true 1
#define false 0

typedef int64_t INumber;
typedef int64_t NeuronId;
typedef int bool;

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif //NEAT_TYPES_H
