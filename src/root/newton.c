#include "root/newton.h"
#include "myutility.h"

int root_itrs = 0;
double root(
    func_t f, func_t dfdx,
    func_t g, func_t dgdx,
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
    } while (abslf(value - prev) >= eps1);

    return value;
}
