#include "vector.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

Vector vec_construct(void) {
    return (Vector) {
        .capacity = 0,
        .length = 0,
        .data = NULL
    };
}

Vector vec_from_capacity(size_t capacity) {
    Vector v = vec_construct();
    vec_reserve_exact(&v, capacity);
    return v;
}

void vec_destruct(Vector *self) {
    for (size_t i = 0; i < self->length; ++i) {
        DESTRUCT_VALUE(&self->data[i]);
    }
    free(self->data);
}


void vec_reserve_exact(Vector *self, size_t additional) {
    assert(additional <= SIZE_MAX - self->capacity);
    self->capacity += additional;
    self->data = realloc(
        self->data,
        sizeof(*self->data) * self->capacity
    );
    assert(self->data);
}

void vec_reserve(Vector *self, size_t additional) {
    if (additional < self->capacity && self->capacity <= SIZE_MAX / 2) {
        additional = self->capacity;
    }
    vec_reserve_exact(self, additional);
}


T *vec_begin(Vector *self) {
    return self->data;
}

T *vec_end(Vector *self) {
    return vec_begin(self) + self->length;
}

T *vec_at(Vector *self, size_t idx) {
    assert(idx < self->length);
    return vec_begin(self) + idx;
}

void vec_push(Vector *self, T value) {
    if (self->length == self->capacity) {
        vec_reserve(self, 1);
    }
    *vec_end(self) = value;
    ++self->length;
}
