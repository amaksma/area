#include "root.h"
#include "myutility.h"
#include <math.h>

int root_itrs = 0;

#ifdef NEWTON
double root(
    func_t f, func_t g,
    func_t dfdx, func_t dgdx,
    double a, double b,
    double eps1
) {
    root_itrs = 0;
    double d2udx2_a = (dfdx(a + eps1) - dgdx(a + eps1) - dfdx(a) + dgdx(a)) / eps1;
    double value = d2udx2_a * (f(a) - g(a)) > 0 ? a : b;
    double prev;
    do {
        prev = value;
        value = prev - (f(prev) - g(prev)) / (dfdx(prev) - dgdx(prev));
        ++root_itrs;
    } while (fabs(value - prev) >= eps1);

    return value;
}
#else
double root(
    func_t f, func_t g,
    double a, double b,
    double eps1
) {
    root_itrs = 0;
    if (fabs(f(a) - g(a)) < eps1) { return a; }
    if (fabs(f(b) - g(b)) < eps1) { return b; }
    while ((b - a) >= eps1) {
        double c = (a + b) / 2;
        if ((f(a) - g(a)) * (f(c) - g(c)) < 0) {
            b = c;
        }
        else {
            a = c;
        }
        ++root_itrs;
    }
    return (a + b) / 2;
}
#endif
