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
