#ifndef INTEGRAL_SQUARE_VEC_H
#define INTEGRAL_SQUARE_VEC_H

#include "myutility.h"
#include "vector.h"

double vec_at_array(Vector *self, size_t idx);

void vec_calculate_array(Vector *self, func_t f, double start, double delta, size_t count);

#endif
