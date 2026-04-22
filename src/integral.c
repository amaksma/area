#include "integral.h"
#include "myutility.h"


double integral(func_t f, double a, double b, double eps2) {
    int k = 10;

    double sum = 0;
    double h = (b - a) / k;
    for (int i = 1; i < k; ++i) {
        sum += f(a + h * i);
    }
    double common = f(a) + 2 * sum + f(b);
    
    double value = 0;
    double prev;
    do {
        prev = value;
        sum = 0;
        h = (b - a) / k;
        for (int i = 0; i < k; ++i) {
            sum += f(a + h * i + h / 2);
        }
        common += 2 * sum;
        value = (common + 2 * sum) * h / 6;
        k *= 2;
    } while (abslf(value - prev) >= eps2);

    return value;
}
