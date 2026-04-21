#ifndef ROOT_NEWTON_H
#define ROOT_NEWTON_H

#include "myutility.h"

extern int root_itrs;
double root(
    func_t f, func_t dfdx,
    func_t g, func_t dgdx,
    double a, double b,
    double eps1
);

#endif
