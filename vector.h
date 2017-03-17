//
// Created by nitori on 16.03.17.
//

#ifndef NEAT_VECTOR_H
#define NEAT_VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define VECTOR_INITIAL_CAPACITY 100
#define VECTOR_GROW_SIZE 100

typedef struct Vector Vector;

struct Vector {
    int32_t size;
    int32_t capacity;
    void** data;
};

Vector* new_vector();
void vector_init(Vector* v);
void vector_append(Vector* v, void* item);
void vector_set(Vector* v, int32_t index, void* item);
void* vector_get(Vector* v, int32_t index);
void vector_grow_if_full(Vector* v);
void vector_free(Vector* v);

#endif //NEAT_VECTOR_H
