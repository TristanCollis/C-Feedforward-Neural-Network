#include <stdlib.h>
#include "matrix.h"


matrix_t mat_new(size_t rows, size_t columns)
{
    matrix_t out = {
        .rows = rows,
        .columns = columns,
        .data = malloc(sizeof(double) * rows * columns),
    };

    return out;
}


double* mat_get(matrix_t *mat, size_t row, size_t column)
{
    return &(mat->data[row * mat->columns + column]);
}

matrix_t mat_mul(matrix_t *mat, matrix_t *b)
{
    matrix_t out = mat_new(mat->rows, b->columns);

    for (int i = 0; i < mat->rows; i++)
    {
        for (int j = 0; j < b->columns; j++)
        {
            double cell = 0;
            for (int k = 0; k < mat->columns; k++)
            {
                cell += *mat_get(mat, i, k) * *mat_get(b, k, j);
            }

            *(mat_get(&out, i, j)) = cell;
        }
    }

    return out;
}

matrix_t mat_from_array(double data[], size_t rows, size_t columns)
{
    matrix_t out = mat_new(rows, columns);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            *mat_get(&out, i, j) = data[i * columns + j];
        }
    }

    return out;
}

matrix_t mat_map(matrix_t *mat, double (*func)(double))
{
    matrix_t out = mat_new()(mat->rows, mat->columns);
    for (size_t i = 0; i < mat->rows; i++)
    {
        for (size_t j = 0; j < mat->columns; j++)
        {
            *mat_get(&out, i, j) = func(*mat_get(mat, i, j));
        }
        
    }
    return out;
}
