#ifndef AREA_H
#define AREA_H

#include "myutility.h"
#include <stdbool.h>

double area(
    func_t f1, func_t f2, func_t f3,
#ifdef NEWTON
    func_t df1dx, func_t df2dx, func_t df3dx,
#endif
    double a12, double b12,
    double a13, double b13,
    double a23, double b23,
    double eps,
    bool print_roots, bool print_root_itrs
);

#endif
