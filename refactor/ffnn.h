#ifndef FFNN_H
#define FFNN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"

typedef struct ffnn_t
{
    uint32_t input_count;
    uint32_t layer_count;
    uint32_t *layer_sizes;
    matrix_t **weights;
    matrix_t **biases;
} ffnn_t;

// Constructor and destructor
ffnn_t *ffnn_new(uint32_t input_count, uint32_t layer_count, uint32_t layer_sizes[]);
void ffnn_free(ffnn_t *nn);

// fread, fwrite
ffnn_t *ffnn_fread(FILE *_Stream);
int ffnn_fwrite(ffnn_t *nn, FILE *_Stream);

// Convenience constructors
ffnn_t *ffnn_rand(uint32_t input_count, uint32_t layer_count, uint32_t layer_sizes[], double min, double max);

// Operations
matrix_t *ffnn_eval(ffnn_t *nn, matrix_t *input);
matrix_t *ffnn_eval_full(ffnn_t *nn, matrix_t *input);


// Convenience functions
void ffnn_printf(FILE *const _Stream, const char *const _Format, ffnn_t *nn);
void ffnn_print(ffnn_t *nn);
uint32_t largest_layer(ffnn_t *nn);

//Trainiing
double ffnn_cost(matrix_t *net_output, matrix_t *exp_output);
void ffnn_train_step(ffnn_t *nn, matrix_t *input, matrix_t *full_output, matrix_t *exp_out, double learning_rate);
void ffnn_train_epoch(ffnn_t *network, uint32_t input_count, matrix_t **inputs, matrix_t **exp_outputs, size_t batch_size, double learning_rate);



#endif