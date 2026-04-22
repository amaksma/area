#include "test.h"
#include "integral.h"
#include "myutility.h"
#include "root.h"
#include <math.h>
#include <stdio.h>

double test_f1(double x) {
    return exp(x) - exp(x * x - 5) / 1000.0 + sin(x * x) - 2;
}
double test_f2(double x) {
    return sin(x * x) * cos(x) + x;
}
double test_f3(double x) {
    return (x == 0 ? 0 : log(sin(abslf(x))) * tan(x) / log(10)) - 0.25;
}

#ifdef NEWTON
double test_df1dx(double x) {
    return exp(x) - x * exp(x * x - 5) / 500.0 + 2 * x * cos(x * x);
}
double test_df2dx(double x) {
    return 2 * x * cos(x) * cos(x * x) - sin(x) * sin(x * x) + 1;
}
double test_df3dx(double x) {
    return x * tan(x) / (abslf(x) * tan(abslf(x))) + log(sin(abslf(x))) / (cos(x) * cos(x));
}
#endif


void test_calculation(void) {
    // Roots
    double test_r12 = -1.12908, test_r13 = 0.36754, test_r23 = -0.14775;

    // Integrals
    double test_i1 = -1.42897047728, test_i2 = -0.346768586812, test_i3 = -0.162565457722;

    double eps = 0.001;
    double eps2 = eps / 3;
    double eps1 = eps2 / 2 / 3;
    

    double x12 = root(
        test_f1, test_f2,
#ifdef NEWTON
        test_df1dx, test_df2dx,
#endif
        -1.5, -0.8, eps1
    );
    double x13 = root(
        test_f1, test_f3,
#ifdef NEWTON
        test_df1dx, test_df3dx,
#endif
        -0.3, 2, eps1
    );
    double x23 = root(test_f2, test_f3,
#ifdef NEWTON
        test_df2dx, test_df3dx,
#endif
        -1, 1, eps1
    );

    printf("expected: %lf\n    x12 = %lf\n", test_r12, x12);
    printf("expected: %lf\n    x13 = %lf\n", test_r13, x13);
    printf("expected: %lf\n    x23 = %lf\n", test_r23, x23);

    double I1 = integral(test_f1, x12, x13, eps2);
    double I2 = integral(test_f2, x12, x23, eps2);
    double I3 = integral(test_f3, x23, x13, eps2);

    printf("expected: %lf\n     I1 = %lf\n", test_i1, I1);
    printf("expected: %lf\n     I2 = %lf\n", test_i2, I2);
    printf("expected: %lf\n     I3 = %lf\n", test_i3, I3);
}
