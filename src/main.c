#include "function/derivative.h"
#include "function/node.h"
#include "function/parser.h"
#include "function/print_tree.h"
#include "function/simplify.h"
#include "integral/square_vec.h"
#include "integral/simpson.h"
#include "myutility.h"
#include "root/newton.h"
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

double df1dx(double x) { return exp(x); }
double df2dx(double x) { return 1.0 / (x * x); }
double df3dx(double x) { return -2.0 / 3.0; }

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
    double a, b;

    func_t f, g, dfdx, dgdx;

    a = -2; b = 2;
    f = f1; g = f2; dfdx = df1dx; dgdx = df2dx;
    double x12 = root(f, dfdx, g, dgdx, a, b, eps1);
    if (print_roots) {
        printf("x12 = %f\n", x12);
    }
    if (print_root_itrs) {
        printf("iterations to calculate x12: %d\n", root_itrs);
    }

    a = -5; b = -3;
    f = f1; g = f3; dfdx = df1dx; dgdx = df3dx;
    double x13 = root(f, dfdx, g, dgdx, a, b, eps1);
    if (print_roots)
        printf("x13 = %f\n", x13);
    if (print_root_itrs) {
        printf("iterations to calculate x13: %d\n", root_itrs);
    }

    a = -5; b = -eps2;
    f = f2; g = f3; dfdx = df2dx; dgdx = df3dx;
    double x23 = root(f, dfdx, g, dgdx, a, b, eps1);
    if (print_roots)
        printf("x23 = %f\n", x23);
    if (print_root_itrs) {
        printf("iterations to calculate x23: %d\n", root_itrs);
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

    //time_t start = clock();
    double I1 = integral(edge_first->f, edge_first->a, edge_first->b, eps2);
    double I2 = integral(edge_second->f, edge_second->a, edge_second->b, eps2);
    double I3 = integral(edge_mid->f, edge_mid->a, edge_mid->b, eps2);
    //time_t end = clock();

    //printf("%ld\n", end - start);


    double I = abslf(I3 - I1 - I2);

    //printf("%f %f %f\n", I1, I2, I3);
    printf("%f\n", I);

    return 0;
}
