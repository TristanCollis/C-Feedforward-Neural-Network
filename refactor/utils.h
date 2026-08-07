#ifndef FFNN_MATH_H
#define FFNN_MATH_H

#define check(val, null_return) \
    if (!val)                   \
    {                           \
        return null_return;     \
    }

#define validate(ptr) \
    if (!ptr)         \
    {                 \
        exit(1);      \
    }

double sigmoid(double x);

#endif