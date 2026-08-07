#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "utils.h"

// Constructor and destructor

matrix_t *mat_new(uint32_t num_rows, uint32_t num_cols)
{
    if (num_rows == 0 || num_cols == 0)
    {
        perror("Invalid dimensions, matrix width or height 0. Expected positive number");
        return NULL;
    }

    matrix_t *matrix = calloc(1, sizeof(*matrix));

    matrix->num_rows = num_rows;
    matrix->num_cols = num_cols;

    matrix->data = calloc(matrix->num_rows, sizeof(*matrix->data));

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        matrix->data[i] = calloc(matrix->num_cols, sizeof(**matrix->data));
    }

    return matrix;
}

void mat_free(matrix_t *matrix)
{
    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        free(matrix->data[i]);
    }

    free(matrix->data);

    free(matrix);
}

// Convenience constructors

matrix_t *mat_copy(matrix_t *matrix)
{
    matrix_t *out = mat_new(matrix->num_rows, matrix->num_cols);

    for (uint32_t i = 0; i < out->num_rows; i++)
    {
        for (uint32_t j = 0; j < out->num_cols; j++)
        {
            out->data[i][j] = matrix->data[i][j];
        }
    }

    return out;
}

matrix_t *mat_range(uint32_t num_rows, uint32_t num_cols, double start, double stop)
{
    matrix_t *matrix = mat_new(num_rows, num_cols);
    check(matrix, NULL);

    double x = start;
    double step = (stop - start) / (double)(matrix->num_rows * matrix->num_cols);

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            matrix->data[i][j] = x;
            x += step;
        }
    }

    return matrix;
}

matrix_t *mat_range_incl(uint32_t num_rows, uint32_t num_cols, double start, double stop)
{
    double steps = (double)(num_rows * num_cols);
    double incl_stop = (stop * steps - start) / (steps - 1);

    matrix_t *matrix = mat_range(num_rows, num_cols, start, incl_stop);

    check(matrix, NULL);

    return matrix;
}

matrix_t *mat_randrange(uint32_t num_rows, uint32_t num_cols, double min, double max)
{
    matrix_t *matrix = mat_new(num_rows, num_cols);
    check(matrix, NULL);

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            matrix->data[i][j] = rand_range(min, max);
        }
    }

    return matrix;
}

// fread, fwrite

matrix_t *mat_fread(FILE *_Stream)
{
    matrix_t *matrix = calloc(sizeof(matrix_t), 1);

    check(fread(&matrix->num_rows, sizeof(matrix->num_rows), 1, _Stream), NULL);
    check(fread(&matrix->num_cols, sizeof(matrix->num_cols), 1, _Stream), NULL);
    
    check(matrix->num_rows, NULL);
    check(matrix->num_cols, NULL);

    matrix->data = calloc(matrix->num_rows, sizeof(*matrix->data));

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        matrix->data[i] = calloc(matrix->num_cols, sizeof(**matrix->data));
        check(fread(matrix->data[i], sizeof(**matrix->data), matrix->num_cols, _Stream), NULL);
    }

    return matrix;
}

int mat_fwrite(matrix_t *matrix, FILE *_Stream)
{
    check(fwrite(&matrix->num_rows, sizeof(matrix->num_rows), 1, _Stream), 0);
    check(fwrite(&matrix->num_cols, sizeof(matrix->num_cols), 1, _Stream), 0);

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {

        check(fwrite(matrix->data[i], sizeof(**matrix->data), matrix->num_cols, _Stream), 0);
    }

    return 1;
}

// Matrix operations

int mat_add_r(matrix_t *lhs, matrix_t *rhs)
{
    if ((lhs->num_rows != rhs->num_rows) || (lhs->num_cols != rhs->num_cols))
    {
        fprintf(
            stderr,
            "Invalid dimensions. Attempted to add [%u x %u] + [%u x %u]",
            lhs->num_rows,
            lhs->num_cols,
            rhs->num_rows,
            rhs->num_cols);
        return 0;
    }

    for (uint32_t i = 0; i < lhs->num_rows; i++)
    {
        for (uint32_t j = 0; j < lhs->num_cols; j++)
        {
            lhs->data[i][j] += rhs->data[i][j];
        }
    }

    return 1;
}

matrix_t *mat_add(matrix_t *lhs, matrix_t *rhs)
{
    matrix_t *matrix = mat_copy(lhs);

    mat_add_r(matrix, rhs);

    return matrix;
}

matrix_t *mat_mul(matrix_t *lhs, matrix_t *rhs)
{
    if (lhs->num_cols != rhs->num_rows)
    {
        fprintf(
            stderr,
            "Invalid dimensions. Attempted to multiply [%u x %u] * [%u x %u]",
            lhs->num_rows,
            lhs->num_cols,
            rhs->num_rows,
            rhs->num_cols);
        return NULL;
    }

    matrix_t *matrix = mat_new(lhs->num_rows, rhs->num_cols);

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            double sum = 0.;

            for (uint32_t k = 0; k < lhs->num_cols; k++)
            {
                sum += lhs->data[i][k] * rhs->data[k][j];
            }

            matrix->data[i][j] = sum;
        }
    }

    return matrix;
}

matrix_t *mat_dot(matrix_t *lhs, matrix_t *rhs)
{
    matrix_t *lhs_trans = mat_transpose(lhs);
    check(lhs_trans, NULL)

        matrix_t *out = mat_mul(lhs_trans, rhs);
    mat_free(lhs_trans);
    check(out, NULL);

    return out;
}

int mat_smul_r(matrix_t *matrix, double scalar)
{
    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            matrix->data[i][j] *= scalar;
        }
    }

    return 1;
}

matrix_t *mat_smul(matrix_t *matrix, double scalar)
{
    matrix_t *out = mat_copy(matrix);

    mat_smul_r(out, scalar);

    return out;
}

matrix_t *mat_transpose(matrix_t *matrix)
{
    matrix_t *out = mat_new(matrix->num_cols, matrix->num_rows);
    check(out, NULL);

    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            out->data[j][i] = matrix->data[i][j];
        }
    }

    return out;
}

int mat_map_r(matrix_t *matrix, double (*fptr)(double))
{
    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            matrix->data[i][j] = fptr(matrix->data[i][j]);
        }
    }

    return 1;
}

matrix_t *mat_map(matrix_t *matrix, double (*fptr)(double))
{
    matrix_t *out = mat_copy(matrix);

    mat_map_r(out, fptr);
    return out;
}

// Convenience matrix functions

void mat_printf(FILE *const _Stream, const char *const _Format, matrix_t *matrix)
{
    for (uint32_t i = 0; i < matrix->num_rows; i++)
    {
        fprintf(_Stream, "\n");
        fprintf(_Stream, (i == 0) ? "[[\t" : " [\t");
        for (uint32_t j = 0; j < matrix->num_cols; j++)
        {
            fprintf(_Stream, _Format, matrix->data[i][j]);
            fprintf(_Stream, "\t");
        }
        fprintf(_Stream, "]");
    }

    fprintf(_Stream, "]\n");
}

void mat_print(matrix_t *matrix)
{
    mat_printf(stdout, "%.2f  ", matrix);
}

// Convenience functions

double rand_range(double min, double max)
{
    double r = (double)rand() / ((double)RAND_MAX + 1.0);
    return min + (max - min) * r;
}
