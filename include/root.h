#ifndef ROOT_H
#define ROOT_H

#include "myutility.h"

extern int root_itrs;
#ifdef NEWTON
// Searches for the root of the difference between f and g using Newton's method
double root(
    func_t f, func_t g,
    func_t dfdx, func_t dgdx,
    double a, double b,
    double eps1
);
#else
// Searches for the root of the difference between f and g using the bisection method
double root(
    func_t f, func_t g,
    double a, double b,
    double eps1
);
#endif

#endif
