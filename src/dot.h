#include <stdlib.h>

double dot(double a[], double b[], uint32_t length)
{
    double result = 0;

    for (int i = 0; i < length; i++)
    {
        result += a[i] * b[i];
    }

    return result;
}
