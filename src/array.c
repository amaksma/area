#include "array.h"
#include <stdlib.h>

ArrayDouble array_from_size(size_t size) {
    return (ArrayDouble) {
        .size = size,
        .data = malloc(size * sizeof(double))
    };
}

void array_destruct(ArrayDouble *self) {
    free(self->data);
}
