#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntVector;

// fills out a new IntVector struct and populates it with init_vals
void create_new(IntVector *iv, int *init_vals, size_t size);

// appends a new int value to the end of the vector
void push_int_back(IntVector *iv, int val);

// reserves the new_size elements worth of memory for the vector
// truncates values past the end if new_size is less than iv->num_elements
void reserve(IntVector *iv, size_t new_capacity);

#endif