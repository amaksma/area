#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include "array.h"

#define DESTRUCT_VALUE(VALUE) array_destruct(VALUE)
typedef ArrayDouble T;

typedef struct {
    size_t capacity;
    size_t length;
    T *data;
} Vector;

Vector vec_construct(void);
Vector vec_from_capacity(size_t capacity);
void vec_destruct(Vector *self);

void vec_reserve_exact(Vector *self, size_t additional);
void vec_reserve(Vector *self, size_t additional);

T *vec_begin(Vector *self);
T *vec_end(Vector *self);
T *vec_at(Vector *self, size_t idx);
void vec_push(Vector *self, T value);

#endif
