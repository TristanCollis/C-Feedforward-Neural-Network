#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

typedef struct matrix_t
{
    uint32_t rows;
    uint32_t columns;
    double *data;
} matrix_t;


double* mat_get(matrix_t *mat, uint32_t row, uint32_t column);
matrix_t mat_mul(matrix_t *lhs, matrix_t *rhs);
matrix_t mat_add(matrix_t *lhs, matrix_t *rhs);
matrix_t mat_new(uint32_t rows, uint32_t columns);
matrix_t mat_from_array(double data[], uint32_t rows, uint32_t columns);

matrix_t mat_map(matrix_t *mat, double (*func)(double));


#endif
