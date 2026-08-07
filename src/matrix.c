#include <stdlib.h>
#include <stdint.h>
#include "matrix.h"


matrix_t mat_new(uint32_t rows, uint32_t columns)
{
    matrix_t out = {
        .rows = rows,
        .columns = columns,
        .data = malloc(sizeof(double) * rows * columns),
    };

    return out;
}


double* mat_get(matrix_t *mat, uint32_t row, uint32_t column)
{
    return &(mat->data[row * mat->columns + column]);
}

matrix_t mat_mul(matrix_t *lhs, matrix_t *rhs)
{
    matrix_t out = mat_new(lhs->rows, rhs->columns);

    for (uint32_t i = 0; i < lhs->rows; i++)
    {
        for (uint32_t j = 0; j < rhs->columns; j++)
        {
            double cell = 0;
            for (uint32_t k = 0; k < lhs->columns; k++)
            {
                cell += *mat_get(lhs, i, k) * *mat_get(rhs, k, j);
            }

            *(mat_get(&out, i, j)) = cell;
        }
    }

    return out;
}

matrix_t mat_add(matrix_t *lhs, matrix_t *rhs)
{
    matrix_t out = mat_new(lhs->rows, lhs->columns);

    for (uint32_t i = 0; i < lhs->rows; i++)
    {
        for (uint32_t j = 0; j < lhs->columns; j++)
        {
            *(mat_get(&out, i, j)) = *mat_get(lhs, i, j) + *mat_get(rhs, i, j);
        }
        
    }
    
    return out;
}

matrix_t mat_from_array(double data[], uint32_t rows, uint32_t columns)
{
    matrix_t out = mat_new(rows, columns);
    for (uint32_t i = 0; i < rows; i++)
    {
        for (uint32_t j = 0; j < columns; j++)
        {
            *mat_get(&out, i, j) = data[i * columns + j];
        }
    }

    return out;
}

matrix_t mat_map(matrix_t *mat, double (*func)(double))
{
    matrix_t out = mat_new(mat->rows, mat->columns);
    for (uint32_t i = 0; i < mat->rows; i++)
    {
        for (uint32_t j = 0; j < mat->columns; j++)
        {
            *mat_get(&out, i, j) = func(*mat_get(mat, i, j));
        }
        
    }
    return out;
}
