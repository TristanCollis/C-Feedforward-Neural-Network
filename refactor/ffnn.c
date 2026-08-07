#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "ffnn.h"
#include "utils.h"

// Constructor and destructor

ffnn_t *ffnn_new(uint32_t input_count, uint32_t layer_count, uint32_t layer_sizes[])
{
    ffnn_t *nn = calloc(1, sizeof(*nn));

    nn->input_count = input_count;
    nn->layer_count = layer_count;

    nn->layer_sizes = calloc(nn->layer_count, sizeof(*nn->layer_sizes));
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        nn->layer_sizes[i] = layer_sizes[i];
    }

    nn->weights = calloc(nn->layer_count, sizeof(*nn->weights));
    nn->weights[0] = mat_new(nn->input_count, nn->layer_sizes[0]);
    for (uint32_t i = 1; i < nn->layer_count; i++)
    {
        nn->weights[i] = mat_new(nn->layer_sizes[i - 1], nn->layer_sizes[i]);
        check(nn->weights[i], NULL);
    }

    nn->biases = calloc(nn->layer_count, sizeof(*nn->biases));
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        nn->biases[i] = mat_new(1, nn->layer_sizes[i]);
        check(nn->biases[i], NULL);
    }

    return nn;
}

void ffnn_free(ffnn_t *nn)
{
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        mat_free(nn->weights[i]);
        mat_free(nn->biases[i]);
    }
    free(nn->weights);
    free(nn->biases);

    free(nn->layer_sizes);

    free(nn);
}

// fread, fwrite

ffnn_t *ffnn_fread(FILE *_Stream)
{
    ffnn_t *nn = calloc(sizeof(ffnn_t), 1);

    check(fread(&nn->input_count, sizeof(nn->input_count), 1, _Stream), NULL);
    check(fread(&nn->layer_count, sizeof(nn->layer_count), 1, _Stream), NULL);

    nn->layer_sizes = calloc(nn->layer_count, sizeof(*nn->layer_sizes));
    check(fread(nn->layer_sizes, sizeof(*nn->layer_sizes), nn->layer_count, _Stream), NULL);

    nn->weights = calloc(nn->layer_count, sizeof(*nn->weights));
    nn->biases = calloc(nn->layer_count, sizeof(*nn->biases));

    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        nn->weights[i] = mat_fread(_Stream);
    }

    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        nn->biases[i] = mat_fread(_Stream);
    }

    return nn;
}

int ffnn_fwrite(ffnn_t *nn, FILE *_Stream)
{
    check(fwrite(&nn->input_count, sizeof(nn->input_count), 1, _Stream), 0);
    check(fwrite(&nn->layer_count, sizeof(nn->layer_count), 1, _Stream), 0);
    check(fwrite(nn->layer_sizes, sizeof(*nn->layer_sizes), nn->layer_count, _Stream), 0);

    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        check(mat_fwrite(nn->weights[i], _Stream), 0);
    }

    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        check(mat_fwrite(nn->biases[i], _Stream), 0);
    }

    return 1;
}

// Convenience constructors

ffnn_t *ffnn_rand(uint32_t input_count, uint32_t layer_count, uint32_t layer_sizes[], double min, double max)
{
    ffnn_t *nn = calloc(1, sizeof(*nn));

    nn->input_count = input_count;
    nn->layer_count = layer_count;

    nn->layer_sizes = calloc(nn->layer_count, sizeof(*nn->layer_sizes));
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        nn->layer_sizes[i] = layer_sizes[i];
    }

    nn->weights = calloc(nn->layer_count, sizeof(*nn->weights));
    nn->weights[0] = mat_randrange(nn->input_count, nn->layer_sizes[0], min, max);
    for (uint32_t i = 1; i < nn->layer_count; i++)
    {
        nn->weights[i] = mat_randrange(nn->layer_sizes[i - 1], nn->layer_sizes[i], min, max);
        check(nn->weights[i], NULL);
    }

    nn->biases = calloc(nn->layer_count, sizeof(*nn->biases));
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        nn->biases[i] = mat_randrange(1, nn->layer_sizes[i], min, max);
        check(nn->biases[i], NULL);
    }

    return nn;
}

// Operations

matrix_t *ffnn_eval(ffnn_t *nn, matrix_t *input)
{
    matrix_t *window, *to_free;

    window = mat_mul(input, nn->weights[0]);
    check(window, NULL);

    // mat_print(window);

    check(mat_add_r(window, nn->biases[0]), NULL);
    check(mat_map_r(window, sigmoid), NULL);

    for (uint32_t i = 1; i < nn->layer_count; i++)
    {
        to_free = window;

        window = mat_mul(window, nn->weights[i]);
        check(mat_add_r(window, nn->biases[i]), NULL);
        check(mat_map_r(window, sigmoid), NULL);

        mat_free(to_free);
    }

    return window;
}

matrix_t *ffnn_eval_full(ffnn_t *nn, matrix_t *input)
{
    matrix_t *output = mat_new(nn->layer_count, largest_layer(nn));
    
    matrix_t *window, *to_free;
    
    window = mat_mul(input, nn->weights[0]);
    check(window, NULL);
    
    for (size_t j = 0; j < nn->layer_sizes[0]; j++)
    {
        output->data[0][j] = window->data[0][j];
    }
    
    
    check(mat_add_r(window, nn->biases[0]), NULL);
    check(mat_map_r(window, sigmoid), NULL);
    
    for (uint32_t i = 1; i < nn->layer_count; i++)
    {
        to_free = window;

        window = mat_mul(window, nn->weights[i]);
        check(mat_add_r(window, nn->biases[i]), NULL);
        check(mat_map_r(window, sigmoid), NULL);

        for (size_t j = 0; j < nn->layer_sizes[i]; j++)
        {
            output->data[i][j] = window->data[0][j];
        }
        mat_free(to_free);
    }

    mat_free(window);

    return output;
}

// Convenience functions

void ffnn_printf(FILE *const _Stream, const char *const _Format, ffnn_t *nn)
{
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        mat_printf(_Stream, _Format, nn->weights[i]);
        mat_printf(_Stream, _Format, nn->biases[i]);
    }
}

void ffnn_print(ffnn_t *nn)
{
    for (uint32_t i = 0; i < nn->layer_count; i++)
    {
        mat_print(nn->weights[i]);
        mat_print(nn->biases[i]);
    }
}

uint32_t largest_layer(ffnn_t *nn)
{
    uint32_t n = 0;

    for (size_t i = 0; i < nn->layer_count; i++)
    {
        n = nn->layer_sizes[i] > n ? nn->layer_sizes[n] : n;
    }

    return n;
}

// Training

double ffnn_cost(matrix_t *net_output, matrix_t *exp_output)
{
    matrix_t *sum = mat_add(net_output, exp_output);

    matrix_t *mat_out = mat_dot(sum, sum);

    double result = mat_out->data[0][0];

    mat_free(sum);
    mat_free(mat_out);

    return result;
}

void ffnn_train_step(ffnn_t *nn, matrix_t *input, matrix_t *full_output, matrix_t *exp_out, double learning_rate)
{
    matrix_t *derivatives = mat_new(nn->layer_count, largest_layer(nn));
    
    uint32_t N = nn->layer_count-1;
    
    for (size_t j = 0; j < nn->layer_sizes[N]; j++)
    {
        derivatives->data[N][j] = 2 * (full_output->data[N][j] - exp_out->data[0][j]);
    }
    

    for (size_t i = N; i > 0; i--)
    {
        for (size_t j = 0; j < nn->layer_sizes[i-1]; j++)
        {
            derivatives->data[i-1][j] = 0;

            for (size_t k = 0; k < nn->layer_sizes[i]; k++)
            {
                derivatives->data[i-1][j] += 
                    derivatives->data[i][k] // a
                    * full_output->data[i][k] * (1 - full_output->data[i][k]) // b
                    * nn->weights[i]->data[j][k]; // c
            }
        }
    }

    for (size_t i = N; i > 0; i--)
    {
        for (size_t k = 0; k < nn->layer_sizes[i]; k++)
        {
            nn->biases[i]->data[0][k] -= 
                derivatives->data[i][k]
                * full_output->data[i][k]
                * (1 - full_output->data[i][k])
                * learning_rate;
            
            
            for (size_t j = 0; j < nn->layer_sizes[i-1]; j++)
            {
                nn->weights[i]->data[j][k] -=
                    derivatives->data[i][k]
                    * full_output->data[i][k]
                    * (1 - full_output->data[i][k])
                    * full_output->data[i - 1][j]
                    * learning_rate;
            }
        }
    }
}

void ffnn_train_epoch(ffnn_t *network, uint32_t input_count, matrix_t **inputs, matrix_t **exp_outputs, size_t batch_size, double learning_rate)
{
    matrix_t **outputs = malloc(input_count * sizeof(matrix_t *));
    
    for (size_t batch = 0; batch < input_count / batch_size; batch++)
    {
        size_t offset = batch * batch_size;
        // run network on batch
        
        for (size_t i = 0; i < batch_size; i++)
        {
            outputs[i] = ffnn_eval_full(network, inputs[offset + i]);
        }
        
        // update network
        
        for (size_t i = 0; i < batch_size; i++)
        {
            ffnn_train_step(network, inputs[offset + i], outputs[i], exp_outputs[offset + i], learning_rate);
        }
    }

    // free outputs

    for (size_t i = 0; i < batch_size; i++)
    {
        mat_free(outputs[i]);
    }

    free(outputs);
}

