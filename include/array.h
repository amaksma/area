#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct {
    size_t size;
    double *data;
} ArrayDouble;

ArrayDouble array_from_size(size_t size);
void array_destruct(ArrayDouble *self);

#endif
