#ifndef FFNN_INCLUDED
#define FFNN_INCLUDED

#include "matrix.h"

typedef struct layer_t
{
    matrix_t weights;
    matrix_t biases;
} layer_t;

typedef struct ffnn_t
{
    layer_t *layers;
    uint32_t length;
} ffnn_t;

ffnn_t *ffnn_new(uint32_t layer_count, uint32_t *shape);
layer_t layer_new(uint32_t layer_size, uint32_t prev_layer_size);


#endif