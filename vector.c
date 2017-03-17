//
// Created by nitori on 16.03.17.
//

#include "vector.h"

Vector* new_vector() {
    Vector* v = calloc(1, sizeof(Vector));
    vector_init(v);
    return v;
}

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

int vector_has(Vector* v, int32_t index) {
    return index >= 0 && index < v->size;
}

void* vector_get(Vector* v, int32_t index) {
    if (!vector_has(v, index)) {
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
