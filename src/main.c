#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dot.h"
#include "ffnn.h"
#include "vector.h"

typedef enum
{
    ARGS_RUN = 1,
    ARGS_CREATE = 2,
    ARGS_TRAIN = 4,
    ARGS_DATASET = 8,
    ARGS_OUTPUT = 16,
    ARGS_NETWORK = 32,
} args;

int main(int argc, char const *argv[])
{
    uint8_t args = 0;

    for (int i = 0; i < argc; i++)
    {
        if (argv[i] == "-r" || argv[i] == "--run")
        {
            args += ARGS_RUN;
        }
        
        else if (argv[i] == "-c" || argv[i] == "--create")
        {
            args += ARGS_CREATE;
        }
        
        else if (argv[i] == "-t" || argv[i] == "--train")
        {
            args += ARGS_TRAIN;
        }
        
        else if (argv[i] == "-d" || argv[i] == "--data" || argv[i] == "--dataset")
        {
            args += ARGS_DATASET;
        }
        
        else if (argv[i] == "-o" || argv[i] == "--out" || argv[i] == "--output")
        {
            args += ARGS_OUTPUT;
        }
        
        else if (argv[i] == "-n" || argv[i] == "--net" || argv[i] == "--network")
        {
            args += ARGS_NETWORK;
        }
    }

    if (args && ARGS_CREATE && ARGS_TRAIN)
    {
        fprintf(stderr, "Invalid argument combination");
        return -1;
    }
    

    return 0;
}
