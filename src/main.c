#include "vector.h"
#include "matrix.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    double data_1[] = {
        1.0, 2.0,
        3.0, 4.0};

    matrix_t a = mat_from_array(data_1, 2, 2);

    double data_2[] = {5.0, 6.0, 7.0, 8.0};
    matrix_t b = mat_from_array(data_2, 2, 2);

    matrix_t product = mat_mul(&a, &b);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.columns; j++)
        {
            printf("%f\n", *mat_get(&product, i, j));
        }
    }

    return 0;
}
