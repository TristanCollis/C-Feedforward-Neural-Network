#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "matrix.h"
#include "ffnn.h"
#include "utils.h"
#include "argparse.h"

#define PI 3.141592653589793238462643383279502884197

void setup()
{
    srand(time(NULL));
    rand();
}

ffnn_t *create()
{
    uint32_t layer_sizes[] = {1, 8, 8, 8, 1};

    ffnn_t *nn = ffnn_rand(
        1, 
        5, 
        layer_sizes, 
        -1.0, 
        1.0
    );
   
    return nn;
}

ffnn_t *open()
{
    FILE *file;
    fopen_s(&file, "model.nn", "rb");
    ffnn_t *nn = ffnn_fread(file);
    fclose(file);
    return nn;
}

void train(ffnn_t *nn)
{
    matrix_t **inputs = malloc(100 * sizeof(matrix_t));
    matrix_t **outputs = malloc(100 * sizeof(matrix_t));

    for (size_t i = 0; i < 100; i++)
    {
        inputs[i] = mat_randrange(1, 1, -PI, PI);
        outputs[i] = mat_new(1, 1);
        outputs[i]->data[0][0] = (sin(2 * inputs[i]->data[0][0]) + 1.0) / 2.0; // training data must be normalized on [0.0, 1.0]
    }
    
    for (size_t i = 0; i < 10000; i++)
    {
        ffnn_train_epoch(nn, 100, inputs, outputs, 100, 0.1);
    }
}

void save(ffnn_t *nn)
{
    FILE *file;
    fopen_s(&file, "model.nn", "wb");
    ffnn_fwrite(nn, file);
    fclose(file);
}

void run(ffnn_t *nn)
{
    for (size_t i = 0; i < 100; i++)
    {
        matrix_t *input = mat_new(1, 1);
        input->data[0][0] = (((double) i) * 2.0 * PI / 100.0) - PI;
        printf("%f, ", input->data[0][0]);
        matrix_t *output = ffnn_eval(nn, input);
        printf("%f\n", output->data[0][0] * 2 - 1);

        mat_free(output);
        mat_free(input);
    }
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 0;
    }

    setup();

    if (strcmp(argv[1], "-c") == 0)
    {
        ffnn_t *nn = create();
        ffnn_print(nn);
        save(nn);
        ffnn_free(nn);
    }

    if (strcmp(argv[1], "-t") == 0)
    {
        ffnn_t *nn = open();
        train(nn);
        save(nn);
        ffnn_free(nn);
    }

    if (strcmp(argv[1], "-r") == 0)
    {
        ffnn_t *nn = open();
        run(nn);
        ffnn_free(nn);
    }

    if (strcmp(argv[1], "-v") == 0)
    {
        ffnn_t *nn = open();

        ffnn_print(nn);
        ffnn_free(nn);
    }

    if (strcmp(argv[1], "-rv") == 0)
    {
        ffnn_t *nn = open();

        matrix_t *input = mat_new(1, 1);
        input->data[0][0] = -1.0;
        
        matrix_t *out = ffnn_eval_full(nn, input);

        mat_print(out);

        mat_free(input);
        mat_free(out);
    }
    
    return 0;
}

