#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>


typedef double (*func_t)(double);

typedef struct {
    double a, b;
    func_t f;
} Segment;

double absd(double value) {
    return value > 0 ? value : -value;
}

double mind(double a, double b) {
    return a < b ? a : b;
}

double maxd(double a, double b) {
    return a > b ? a : b;
}

double midd(double a, double b, double c) {
    if (a > b) {
        double tmp = a;
        a = b;
        b = tmp;
    }
    if (a < c) { return mind(b, c); }
    return a;
}

static int root_itrs = 0;
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
    } while (absd(value - prev) >= eps1);

    return value;
}


struct node {
    double key;
    struct node *next;
};

struct node *new(void) {
    return malloc(sizeof(struct node));
}

struct node *insert_after(struct node *root, double key) {
    assert(root);
    struct node *ptr = new();
    *ptr = (struct node) { .key = key, .next = root->next };
    root->next = ptr;
    return ptr;
}

void free_list(struct node *root) {
    if (!root) { return; }
    free_list(root->next);
    free(root);
}

struct node *generate_list(func_t f, double a, double b, int k) {
    struct node *root = new();
    struct node *end = root;
    *root = (struct node) { .key = f(a), .next = NULL };

    int N = k * 2;
    double h = (b - a) / N;
    double xi = a;
    for (int i = 0; i < N; ++i) {
        xi += h;
        end = insert_after(end, f(xi));
    }
    return root;
}

double simpson_precomputed(func_t f, double a, double b, int k, struct node *root) {
    int N = k * 2;
    double h = (b - a) / N;
    double xi = a + h;
    double value = 0;
    for (int i = 0; i < k; ++i) {

        value += root->key;
        root = insert_after(root, f(xi));
        value += root->key * 4;
        root = root->next;
        value += root->key;

        xi += 2 * h;
    }

    return value * h / 3;
}

double simpson_simple(func_t f, double a, double b, int k) {
    int N = k * 2;
    double h = (b - a) / N;
    double value = 0;
    for (int i = 1; i <= k; ++i) {
        value += f(a + (2 * i - 2) * h);
        value += f(a + (2 * i - 1) * h) * 4;
        value += f(a + 2 * i * h);
    }
    return value * h / 3;
}

double simpson(func_t f, double a, double b, double eps2) {
    int k = 5;
    
    struct node *root = generate_list(f, a, b, k);

    double value = 0;
    double prev;
    do {
        k *= 2;
        prev = value;
        value = simpson_precomputed(f, a, b, k, root);
    } while (absd(value - prev) >= eps2);

    free_list(root);
    return value;
}

double integral(func_t f, double a, double b, double eps2) {
    int k = 5;
    double value = 0;
    double prev;
    do {
        k *= 2;
        prev = value;
        value = simpson_simple(f, a, b, k);
    } while (absd(value - prev) >= eps2);

    return value;
}

double f1(double x) { return exp(x) + 2.0; }
double f2(double x) { return -1.0 / x; }
double f3(double x) { return -2.0 * (x + 1.0) / 3.0; }

double df1dx(double x) { return exp(x); }
double df2dx(double x) { return 1.0 / (x * x); }
double df3dx(double x) { return -2.0 / 3.0; }



int main(int argc, char *argv[]) {
    // Handling of the command line arguments
    const char arg_help[] = "-help";
    const char arg_roots[] = "-roots";
    const char arg_iterations[] = "-iterations";
    const char *possible_args[] = {arg_help, arg_roots, arg_iterations};

    bool print_roots = false;
    bool print_root_itrs = false;

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


    Segment edge1 = (Segment) { mind(x12, x13), maxd(x12, x13), f1 };
    Segment edge2 = (Segment) { mind(x12, x23), maxd(x12, x23), f2 };
    Segment edge3 = (Segment) { mind(x13, x23), maxd(x13, x23), f3 };

    double mid_x = midd(x12, x13, x23);

    Segment *edge_mid = &edge1;
    if (edge_mid->a == mid_x || edge_mid->b == mid_x) { edge_mid = &edge2; }
    if (edge_mid->a == mid_x || edge_mid->b == mid_x) { edge_mid = &edge3; }

    Segment *edge_first = &edge1 != edge_mid ? &edge1 : &edge2;

    Segment *edge_second = &edge1;
    if (edge_second == edge_mid || edge_second == edge_first) { edge_second = &edge2; }
    if (edge_second == edge_mid || edge_second == edge_first) { edge_second = &edge3; }

    time_t start = clock();
    double I1 = simpson(edge_first->f, edge_first->a, edge_first->b, eps2);
    double I2 = simpson(edge_second->f, edge_second->a, edge_second->b, eps2);
    double I3 = simpson(edge_mid->f, edge_mid->a, edge_mid->b, eps2);
    time_t end = clock();

    printf("%ld\n", end - start);


    double I = absd(I3 - I1 - I2);

    //printf("%f %f %f\n", I1, I2, I3);
    printf("%f\n", I);

    return 0;
}