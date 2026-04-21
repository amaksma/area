#include "myutility.h"
#include <assert.h>
#include <stdint.h>

inline size_t count_chars(const char *str, char ch) {
    size_t count = 0;
    while (*str) {
        if (*str == ch) { ++count; }
        ++str;
    }
    return count;
}

unsigned trailing_zeros(uint32_t value) {
    value &= ~value + 1;
    unsigned count = 32;
    if (value != 0) { --count; }
    if (value & 0x0000FFFF) { count -= 16; }
    if (value & 0x00FF00FF) { count -= 8; }
    if (value & 0x0F0F0F0F) { count -= 4; }
    if (value & 0x33333333) { count -= 2; }
    if (value & 0x55555555) { count -= 1; }
    return count;
}

double abslf(double value) {
    return value > 0 ? value : -value;
}

double minlf(double a, double b) {
    return a < b ? a : b;
}

double maxlf(double a, double b) {
    return a > b ? a : b;
}

double midlf(double a, double b, double c) {
    if (a > b) { swaplf(&a, &b); }
    if (a < c) { return minlf(b, c); }
    return a;
}

void swaplf(double *a, double *b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}
