#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "ffnn.h"
#include "vector.h"

#define perror(str) fprintf(stderr, str)

typedef enum
{
    ARGS_RUN = 1,
    ARGS_CREATE = 2,
    ARGS_TRAIN = 4,
    ARGS_DATASET = 8,
    ARGS_OUTPUT = 16,
    ARGS_NETWORK = 32,
} args;

int main(int argc, char *argv[])
{
    uint8_t args = 0;
    uint32_t *nn_shape;
    char *nn_name;

    ffnn_t *model;

    for (uint32_t i = 0; (int)i < argc; i++)
    {
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--run") == 0)
        {
            args |= ARGS_RUN;
        }

        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--create") == 0)
        {
            args |= ARGS_CREATE;

            if ((int)i >= argc)
            {
                perror("No value provided for argument");
                return -1;
            }

            uint32_t layer_count = 0;
            bool last_is_digit = false;
            for (uint32_t j = 0; j < strlen(argv[i + 2]); j++)
            {
                if (last_is_digit && !isdigit(argv[i + 2][j]))
                {
                    last_is_digit = false;
                }

                if (!last_is_digit && isdigit(argv[i + 2][j]))
                {
                    last_is_digit = true;
                    layer_count++;
                }
            }

            nn_shape = calloc(layer_count, sizeof(uint32_t));

            uint32_t k = 0;
            last_is_digit = false;
            for (uint32_t j = 0; j < strlen(argv[i + 2]); j++)
            {
                if (last_is_digit && !isdigit(argv[i + 2][j]))
                {
                    last_is_digit = false;
                    k++;
                }

                if (!last_is_digit && isdigit(argv[i + 2][j]))
                {
                    last_is_digit = true;
                    nn_shape[k] += atoi(&argv[i + 2][j]);
                }
            }

            nn_name = argv[i + 1];



            for (uint32_t j = 0; j < layer_count; j++)
            {
                printf("%d\n", nn_shape[j]);
            }
        }

        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--train") == 0)
        {
            args += ARGS_TRAIN;
        }

        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--data") || strcmp(argv[i], "--dataset") == 0)
        {
            args += ARGS_DATASET;
        }

        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0 || strcmp(argv[i], "--output") == 0)
        {
            args += ARGS_OUTPUT;
        }

        else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--net") == 0 || strcmp(argv[i], "--network") == 0)
        {
            args += ARGS_NETWORK;
        }
    }

    if (((args & ARGS_TRAIN) && (args & ARGS_RUN)) || ((args & ARGS_CREATE) && (args & ARGS_NETWORK)))
    {
        perror("Invalid argument combination");
        return -1;
    }

    if (args & ARGS_CREATE) 
    {
        // FILE *fptr = fopen("./file.nn", "w");
        // fwrite(&num, sizeof(uint32_t), 1, fptr);
        // fprintf(fptr, "Bleh");
        // fclose(fptr);
    }

    return 0;
}


// data binary layout:
// Header: u32 - number of layers
// layer:
//      sub-header: u32 - size of layer
//      array of doubles