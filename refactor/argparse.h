#include <stdint.h>

typedef struct context_t
{
    uint32_t input_count;
    uint32_t layer_count;
    uint32_t *layer_sizes;

    const char *in_path;
    const char *out_path;

    double *input;
    double *expected_output;

} context_t;

typedef enum opmode_t
{
    OPMODE_UNDETERMINED,
    OPMODE_CREATE,
    OPMODE_TRAIN,
    OPMODE_RUN,
    OPMODE_INVALID
} opmode_t;

void print_usage(char *program_name);

int flag_present(int argc, char *argv[], char *flag);

opmode_t get_opmode(int argc, char *argv[]);

int arg_index(int argc, char *argv[], char *opt);

context_t *get_context(int argc, char *argv[], opmode_t opmode);
