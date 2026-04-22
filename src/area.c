#include "area.h"
#include "integral.h"
#include "root.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    double a, b;
    func_t f;
} Segment;

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
) {
    double eps2 = eps / 3;
    double eps1 = eps2 / 2 / 3;
    int root_itrs12, root_itrs13, root_itrs23;
    
#ifdef NEWTON
    double x12 = root(f1, df1dx, f2, df2dx, a12, b12, eps1);
    root_itrs12 = root_itrs;
    double x13 = root(f1, df1dx, f3, df3dx, a13, b13, eps1);
    root_itrs13 = root_itrs;
    double x23 = root(f2, df2dx, f3, df3dx, a23, b23, eps1);
    root_itrs23 = root_itrs;
#else
    double x12 = root(f1, f2, a12, b12, eps1);
    root_itrs12 = root_itrs;
    double x13 = root(f1, f3, a13, b13, eps1);
    root_itrs13 = root_itrs;
    double x23 = root(f2, f3, a23, b23, eps1);
    root_itrs23 = root_itrs;
#endif

    if (print_roots) {
        printf("x12 = %f\n", x12);
        printf("x13 = %f\n", x13);
        printf("x23 = %f\n", x23);
    }
    if (print_root_itrs) {
        printf("Iterations to calculate x12: %d\n", root_itrs12);
        printf("Iterations to calculate x13: %d\n", root_itrs13);
        printf("Iterations to calculate x23: %d\n", root_itrs23);
    }


    Segment edge1 = (Segment) { minlf(x12, x13), maxlf(x12, x13), f1 };
    Segment edge2 = (Segment) { minlf(x12, x23), maxlf(x12, x23), f2 };
    Segment edge3 = (Segment) { minlf(x13, x23), maxlf(x13, x23), f3 };

    double mid_x = midlf(x12, x13, x23);

    Segment *edge_mid = &edge1;
    if (edge_mid->a == mid_x || edge_mid->b == mid_x) { edge_mid = &edge2; }
    if (edge_mid->a == mid_x || edge_mid->b == mid_x) { edge_mid = &edge3; }

    Segment *edge_first = &edge1 != edge_mid ? &edge1 : &edge2;

    Segment *edge_second = &edge1;
    if (edge_second == edge_mid || edge_second == edge_first) { edge_second = &edge2; }
    if (edge_second == edge_mid || edge_second == edge_first) { edge_second = &edge3; }

    double I1 = integral(edge_first->f, edge_first->a, edge_first->b, eps2);
    double I2 = integral(edge_second->f, edge_second->a, edge_second->b, eps2);
    double I3 = integral(edge_mid->f, edge_mid->a, edge_mid->b, eps2);

    return abslf(I1 + I2 - I3);
}