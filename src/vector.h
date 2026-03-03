#include <stdlib.h>

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#define VEC_DEFAULT_CAPACITY 4

typedef struct vec_t
{
    double *_data;
    size_t _length;
    size_t _capacity;
} vec_t;

vec_t vec_new();
void vec_push(vec_t *vec, double elem);
double vec_get(vec_t *vec, int index);
double vec_pop(vec_t *vec);
int vec_length(vec_t *vec);
void vec_clear(vec_t *vec);

#endif