#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

typedef struct matrix_t
{
    size_t rows;
    size_t columns;
    double *data;
} matrix_t;

// #define mat_get(mat, i, j) (mat.data[i * mat.columns + j])

double* mat_get(matrix_t *mat, size_t row, size_t column);
matrix_t mat_mul(matrix_t *lhs, matrix_t *rhs);
matrix_t mat_new(size_t rows, size_t columns);
matrix_t mat_from_array(double data[], size_t rows, size_t columns);

matrix_t mat_map(matrix_t *mat, double (*func)(double));


#endif
