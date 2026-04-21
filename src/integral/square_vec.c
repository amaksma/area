#include "integral/square_vec.h"
#include "myutility.h"
#include <stddef.h>
#include <stdint.h>

double vec_at_array(Vector *self, size_t idx) {
    size_t k = idx == 0 ? self->length : trailing_zeros((uint32_t)idx) + 1;
    k = k > self->length ? self->length : k;
    size_t vec_idx = self->length - k;
    if (vec_idx == 0) { --k; }
    size_t arr_idx = idx >> k;
    
    return vec_at(self, vec_idx)->data[arr_idx];
}

void vec_calculate_array(Vector *self, func_t f, double start, double delta, size_t count) {
    ArrayDouble array = array_from_size(count);
    for (size_t i = 0; i < count; ++i) {
        array.data[i] = f(start);
        start += delta;
    }
    vec_push(self, array);
}
