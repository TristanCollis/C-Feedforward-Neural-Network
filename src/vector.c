#include "vector.h"

static const size_t DEFAULT_CAPACITY = 4;

vec_t vec_new()
{
    vec_t out = (vec_t){
        ._data = malloc(DEFAULT_CAPACITY * sizeof(double)),
        ._length = 0,
        ._capacity = DEFAULT_CAPACITY};

    return out;
}

void vec_push(vec_t *vec, double elem)
{
    vec->_length++;

    if (vec->_length > vec->_capacity)
    {
        vec->_capacity = vec->_length * 2;
        vec->_data = realloc(vec->_data, vec->_capacity * sizeof(double));
    }

    vec->_data[vec->_length - 1] = elem;
}

double vec_get(vec_t *vec, int index)
{
    return vec->_data[index];
}

double vec_pop(vec_t *vec)
{
    return vec->_data[--vec->_length];
}

void vec_clear(vec_t *vec)
{
    free(vec->_data);
    vec->_length=0;
    vec->_capacity=1;
}

int vec_length(vec_t *vec)
{
    return vec->_length;
}