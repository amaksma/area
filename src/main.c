#include "function/derivative.h"
#include "function/node.h"
#include "function/parser.h"
#include "function/print_tree.h"
#include "function/simplify.h"
#include "integral/square_vec.h"
#include "integral/simpson.h"
#include "myutility.h"
#include "root.h"
#include "vector.h"
#include "array.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void print_vec(Vector *self) {
    size_t len = 0;
    for (size_t i = 0; i < self->length; ++i) {
        len += vec_at(self, i)->size;
    }
    for (size_t i = 0; i < len; ++i) {
        printf("%.2lf ", vec_at_array(self, i));
    }
    fputc('\n', stdout);
}

typedef struct {
    double a, b;
    func_t f;
} Segment;


double f1(double x) { return exp(x) + 2.0; }
double f2(double x) { return -1.0 / x; }
double f3(double x) { return -2.0 * (x + 1.0) / 3.0; }

#ifdef NEWTON
double df1dx(double x) { return exp(x); }
double df2dx(double x) { return 1.0 / (x * x); }
double df3dx(double x) { return -2.0 / 3.0; }
#endif

int main1232(void) {
    Node *root = construct_tree("x x x * 5 - 1000 / - x x * sin + 2 -");
    print_tree(root);

    Node *droot = derivative(root, 'x');

    print_tree(droot);
    droot = simplify(droot);
    print_tree(droot);
    destruct_tree(droot);
    destruct_tree(root);

    Vector vec = vec_from_capacity(10);
    size_t n = 2;
    ArrayDouble arr1 = array_from_size(3);
    arr1.data[0] = 0;
    arr1.data[1] = 4;
    arr1.data[2] = 8;
    vec_push(&vec, arr1);
    print_vec(&vec);
    while (1) {
        ArrayDouble a = array_from_size(n);
        for (size_t i = 0; i < n; ++i) {
            scanf("%lf", &a.data[i]);
        }
        vec_push(&vec, a);
        print_vec(&vec);
        n *= 2;
    }
    vec_destruct(&vec);
    

    return 0;
}

int main(int argc, char *argv[]) {
    // Handling of the command line arguments
    const char arg_help[] = "-help";
    const char arg_roots[] = "-roots";
    const char arg_iterations[] = "-iterations";
    const char arg_test[] = "-test";
    const char *possible_args[] = {arg_help, arg_roots, arg_iterations, arg_test};

    bool print_roots = false;
    bool print_root_itrs = false;
    bool test = false;

    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];
        if (strcmp(arg, arg_help) == 0) {
            for (size_t j = 0; j < sizeof(possible_args) / sizeof(*possible_args); ++j) {
                fputs(possible_args[j], stdout);
                fputc(' ', stdout);
            }
            fputc('\n', stdout);
        }
        else if (strcmp(arg, arg_roots) == 0) {
            print_roots = true;
        }
        else if (strcmp(arg, arg_iterations) == 0) {
            print_root_itrs = true;
        }
        else if (strcmp(arg, arg_test) == 0) {
            test = true;
        }
    }

    double eps = 0.001;
    double eps2 = eps / 3;
    double eps1 = eps2 / 2 / 3;

    double a12 = -2, b12 = -eps2;
    double a13 = -5, b13 = -3;
    double a23 = -5, b23 = -eps2;
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

    double I = abslf(I1 + I2 - I3);

    //printf("%f %f %f\n", I1, I2, I3);
    printf("%f\n", I);

    return 0;
}
