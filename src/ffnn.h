#ifndef FFNN_INCLUDED
#define FFNN_INCLUDED

#include "matrix.h"


typedef struct layer_t
{
    matrix_t weights;
    matrix_t biases;
}


#endif