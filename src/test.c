#include "test.h"
#include "myutility.h"
#include "integral.h"
#include "root.h"
#include "area.h"
#include <assert.h>
#include <math.h>


static double f1(double x) {
    return exp(x) - exp(x * x - 5) / 1000.0 + sin(x * x) - 2;
}
static double f2(double x) {
    return sin(x * x) * cos(x) + x;
}
static double f3(double x) {
    return (x == 0 ? 0 : log(sin(abslf(x))) * tan(x) / log(10)) - 0.25;
}

#ifdef NEWTON
static double df1dx(double x) {
    return exp(x) - x * exp(x * x - 5) / 500.0 + 2 * x * cos(x * x);
}
static double df2dx(double x) {
    return 2 * x * cos(x) * cos(x * x) - sin(x) * sin(x * x) + 1;
}
static double df3dx(double x) {
    return x * tan(x) / (abslf(x) * tan(abslf(x))) + log(sin(abslf(x))) / (cos(x) * cos(x));
}
#endif


void test_calculation(void) {
    // Roots
    double expected_x12 = -1.1290793957594790364306327, expected_x13 = 0.367539900645641914785130356, expected_x23 = -0.147751873920642938090099763;

    // Integrals
    double expected_I1 = -1.4289838058025445681963979, expected_I2 = -0.3467679152384785995901733, expected_I3 = 0.16256565271856085;
    double expected_I = abslf(expected_I1) - abslf(expected_I2) - abslf(expected_I3);

    double eps = 0.00000001;
    double eps2 = eps / 3;
    double eps1 = eps2 / 2 / (f2(expected_x23) - f2(expected_x12));

    double a12 = -1.5, b12 = -0.8;
    double a13 = -0.3, b13 = 2;
    double a23 = -1, b23 = 1;
    

    double x12 = root(
        f1, f2,
#ifdef NEWTON
        df1dx, df2dx,
#endif
        a12, b12, eps1
    );
    double x13 = root(
        f1, f3,
#ifdef NEWTON
        df1dx, df3dx,
#endif
        a13, b13, eps1
    );
    double x23 = root(f2, f3,
#ifdef NEWTON
        df2dx, df3dx,
#endif
        a23, b23, eps1
    );


    double I1 = integral(f1, x12, x13, eps2);
    double I2 = integral(f2, x12, x23, eps2);
    double I3 = integral(f3, x13, x23, eps2);

    double I = area(
        f1, f2, f3,
#ifdef NEWTON
        df1dx, df2dx, df3dx,
#endif
        a12, b12, a13, b13, a23, b23, eps, false, false
    );

    assert(abslf(x12 - expected_x12) < eps1);
    assert(abslf(x13 - expected_x13) < eps1);
    assert(abslf(x23 - expected_x23) < eps1);
    assert(abslf(I1 - expected_I1) < eps2);
    assert(abslf(I2 - expected_I2) < eps2);
    assert(abslf(I3 - expected_I3) < eps2);
    assert(abslf(I - expected_I) < eps);
}
