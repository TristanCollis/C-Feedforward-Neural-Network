#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct matrix_t
{
    uint32_t num_rows;
    uint32_t num_cols;
    double **data;
} matrix_t;


// Basic constructor and destructor

matrix_t *mat_new(uint32_t num_rows, uint32_t num_cols);
void mat_free(matrix_t *matrix);

// fread, fwrite

matrix_t *mat_fread(FILE *_Stream);
int mat_fwrite(matrix_t *matrix, FILE *_Stream);

// Convenience constructors

matrix_t *mat_copy(matrix_t *matrix);
matrix_t *mat_range(uint32_t num_rows, uint32_t num_cols, double start, double stop);
matrix_t *mat_range_incl(uint32_t num_rows, uint32_t num_cols, double start, double stop);
matrix_t *mat_randrange(uint32_t num_rows, uint32_t num_cols, double min, double max);

// Matrix operations
// _r denotes modification by reference (in-place operation).
// Operations without _r allocate new matrices.

int mat_add_r(matrix_t *lhs, matrix_t *rhs);
matrix_t *mat_add(matrix_t *lhs, matrix_t *rhs);

matrix_t *mat_mul(matrix_t *lhs, matrix_t *rhs);
matrix_t *mat_dot(matrix_t *lhs, matrix_t *rhs);

int mat_smul_r(matrix_t *matrix, double scalar);
matrix_t *mat_smul(matrix_t *matrix, double scalar);

matrix_t *mat_transpose(matrix_t *matrix);

int mat_map_r(matrix_t *matrix, double (*fptr)(double));
matrix_t *mat_map(matrix_t *matrix, double (*fptr)(double));

// Convenience matrix functions

void mat_printf(FILE *const _Stream, const char *const _Format, matrix_t *matrix);
void mat_print(matrix_t *matrix);

// Convenience functions

double rand_range(double min, double max);

#endif