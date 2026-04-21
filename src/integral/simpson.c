#include "integral/simpson.h"
#include "integral/square_vec.h"
#include "myutility.h"
#include "vector.h"


static double simpson_precomputed(func_t f, double a, double b, size_t k, Vector *vec) {
    double h = (b - a) / (double)(k * 2);
    
    vec_calculate_array(vec, f, a + h, h * 2, k);
    
    double value = 0;
    for (size_t i = 0; i < k; ++i) {
        value += vec_at_array(vec, 2 * i);
        value += vec_at_array(vec, 2 * i + 1) * 4;
        value += vec_at_array(vec, 2 * i + 2);
    }

    return value * h / 3;
}

double integral(func_t f, double a, double b, double eps2) {
    size_t k = 5;

    size_t N = k * 2;
    double h = (b - a) / (double)N;
    Vector vec = vec_from_capacity(100);
    vec_calculate_array(&vec, f, a, h, N + 1);

    double value = 0;
    double prev;
    do {
        k *= 2;
        prev = value;
        value = simpson_precomputed(f, a, b, k, &vec);
    } while (abslf(value - prev) >= eps2);

    vec_destruct(&vec);
    return value;
}
