//
// Created by nitori on 16.03.17.
//

#include "vector.h"

void vector_init(Vector* v) {
    v->capacity = VECTOR_INITIAL_CAPACITY;
    v->size = 0;
    v->data = calloc(1, sizeof(int*) * v->capacity);
}

void vector_append(Vector* v, void* item) {
    vector_grow_if_full(v);
    v->data[v->size] = item;
    v->size++;
}

void vector_set(Vector* v, int32_t index, void* item) {
    while (index >= v->size) {
        vector_append(v, NULL);
    }
    v->data[index] = item;
}

void* vector_get(Vector* v, int32_t index) {
    if (index >= v->size || index < 0) {
        fprintf(stderr, "Index %d out of bounds for vector of size %d\n", index, v->size);
        return NULL;
    }
    return v->data[index];
}

void vector_grow_if_full(Vector* v) {
    if (v->size < v->capacity) {
        return;
    }
    v->capacity += VECTOR_GROW_SIZE;
    v->data = realloc(v->data, sizeof(int*) * v->capacity);
}

void vector_free(Vector* v) {
    free(v->data);
}
