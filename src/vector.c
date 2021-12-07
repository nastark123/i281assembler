#include "vector.h"

void create_new(IntVector *iv, int *init_vals, size_t size) {
    iv->data = calloc(size, sizeof(int));
    iv->size = 0;
    iv->capacity = size;
    if(init_vals != NULL) {
        iv->size = size;
        for(int i = 0; i < size; i++) {
            iv->data[i] = init_vals[i];
        }
    }
}

void push_int_back(IntVector *iv, int val) {
    // check to see if adding to the end would write past the allocated memory
    if(iv->size >= iv->capacity) {
        iv->data = realloc(iv->data, (iv->size > 256) ? 2 * iv->capacity : iv->capacity + (4 * sizeof(int)));
    }

    iv->data[iv->size++] = val;   
}

void reserve(IntVector *iv, size_t new_capacity) {
    iv->capacity = new_capacity;
    if(iv->size < new_capacity) iv->size = new_capacity;

    iv->data = realloc(iv->data, new_capacity * sizeof(int));
}