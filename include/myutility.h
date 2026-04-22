#ifndef MYUTILITY_H
#define MYUTILITY_H

#include <stddef.h>
#include <stdint.h>

#define C_PI 3.1415926535897932384626433832795
#define C_E  2.7182818284590452353602874713527


typedef double (*func_t)(double);

// Counts the number of occurrences of a character in a string
size_t count_chars(const char *str, char ch);

double abslf(double value);
double minlf(double a, double b);
double maxlf(double a, double b);
double midlf(double a, double b, double c);
void swaplf(double *a, double *b);

#endif
