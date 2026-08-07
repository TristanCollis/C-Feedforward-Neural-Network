#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "argparse.h"

void print_usage(char *program_name)
{
    printf(
        "Usage: \n"
        "%s {-c|-t|-r} [...]\n\n",
        program_name);
    printf(
        "-c:\tCreate\n"
        "\t-i <input count>\n"
        "\t-l <layer count>\n"
        "\t-d <layer sizes>\n"
        "\t-o <output path>\n"

        "-t:\tTrain\n"
        "\t-v <input values>\n"
        "\t-n <model math>\n"
        "\t-e <expected output>\n"

        "-r:\tRun\n"
        "\t-v <input values>\n"
        "\t-n <model path>\n");
}

int flag_present(int argc, char *argv[], char *flag)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], flag) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int arg_index(int argc, char *argv[], char *opt)
{
    for (int i = 1; i < (argc - 1); i++)
    {
        if (strcmp(argv[i], opt) == 0)
        {
            return i + 1;
        }
    }

    return -1;
}

opmode_t get_opmode(int argc, char *argv[])
{
    opmode_t opmode = OPMODE_UNDETERMINED;

    if (flag_present(argc, argv, "-c"))
    {
        opmode = (opmode == OPMODE_UNDETERMINED) ? OPMODE_CREATE : OPMODE_INVALID;
    }
    if (flag_present(argc, argv, "-t"))
    {
        opmode = (opmode == OPMODE_UNDETERMINED) ? OPMODE_TRAIN : OPMODE_INVALID;
    }
    if (flag_present(argc, argv, "-r"))
    {
        opmode = (opmode == OPMODE_UNDETERMINED) ? OPMODE_RUN : OPMODE_INVALID;
    }

    return opmode;
}

context_t *get_context(int argc, char *argv[], opmode_t opmode)
{
    context_t *context = calloc(1, sizeof(context_t));

    switch (opmode)
    {
    case OPMODE_CREATE:
        printf("Create\n");
        break;

    case OPMODE_TRAIN:
        printf("Train\n");
        break;

    case OPMODE_RUN:
        printf("Run\n");
        break;

    default:
        free(context);
        return NULL;
        break;
    }

    return context;
}
