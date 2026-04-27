#include "myutility.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

inline size_t count_chars(const char *str, char ch) {
    size_t count = 0;
    while (*str) {
        if (*str == ch) { ++count; }
        ++str;
    }
    return count;
}

double fmid(double a, double b, double c) {
    if (a > b) { fswap(&a, &b); }
    if (a < c) { return fmin(b, c); }
    return a;
}

void fswap(double *a, double *b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}
