#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "area.h"
#include "integral.h"
#include "root.h"
#include "test.h"





double f1(double x) { return exp(x) + 2.0; }
double f2(double x) { return -1.0 / x; }
double f3(double x) { return -2.0 * (x + 1.0) / 3.0; }

#ifdef NEWTON
double df1dx(double x) { return exp(x); }
double df2dx(double x) { return 1.0 / (x * x); }
double df3dx(double x) { return -2.0 / 3.0; }
#endif


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

    if (test) {
        fputs(
            "Switched to testing mode\n"
            "Write 'q' to exit the testing mode\n"
            "Write 'r' to test the root search\n"
            "Write 'i' to test the integral calculation\n"
            "Write 't' to execute the automatic tests\n",
            stdout
        );
    }
    while (test) {
        fputs("command: ", stdout);
        char cmd[2];
        scanf("%1s", cmd);
        switch (*cmd) {
        case 'r': {
            fputs("Input format: <first func number> <second func number> <beginning of the segment> <end of the segment> <epsilon>\n", stdout);
            int f_num, g_num;
            double a, b, eps;
            scanf("%d %d %lf %lf %lf", &f_num, &g_num, &a, &b, &eps);

            double x = root(
                f_num == 1 ? f1 : f_num == 2 ? f2 : f3,
                g_num == 1 ? f1 : g_num == 2 ? f2 : f3,
#ifdef NEWTON
                f_num == 1 ? df1dx : f_num == 2 ? df2dx : df3dx,
                g_num == 1 ? df1dx : g_num == 2 ? df2dx : df3dx,
#endif
                a, b, eps
            );

            printf("root = %lf\n", x);
            if (print_root_itrs) {
                printf("Iterations: %d\n", root_itrs);
            }
            break;
        }
        case 'i': {
            fputs("Input format: <function number> <beginning of the segment> <end of the segment> <epsilon>\n", stdout);
            int f_num;
            double a, b, eps;
            scanf("%d %lf %lf %lf", &f_num, &a, &b, &eps);
            double I = integral(f_num == 1 ? f1 : f_num == 2 ? f2 : f3, a, b, eps);
            printf("I = %lf\n", I);
            break;
        }
        case 't':
            test_calculation();
            break;
        case 'q':
            test = false;
            break;
        }
    }

    double eps = 0.001;

    double a12 = -2, b12 = -eps;
    double a13 = -5, b13 = -3;
    double a23 = -5, b23 = -eps;

    double I = area(
        f1, f2, f3,
#ifdef NEWTON
        df1dx, df2dx, df3dx,
#endif
        a12, b12, a13, b13, a23, b23,
        eps, print_roots, print_root_itrs
    );
    printf("%lf\n", I);

    return 0;
}
