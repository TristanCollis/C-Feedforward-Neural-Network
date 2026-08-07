#include <stdint.h>
#include <stdlib.h>
#include "ffnn.h"




ffnn_t *ffnn_new(uint32_t layer_count, uint32_t *shape)
{
    ffnn_t out = {
        .length = layer_count,
        .layers = malloc()
    };
    for (uint32_t i = 1; i < layer_count; i++)
    {
        uint32_t layer_size = shape[i];
        uint32_t prev_layer_size = shape[i-1];
        

    }
    
}

layer_t layer_new(uint32_t layer_size, uint32_t prev_layer_size)
{
    layer_t out = 
    {
        .weights = mat_new(prev_layer_size, layer_size),
        .biases = mat_new(prev_layer_size, layer_size)
    };

    return out;
}