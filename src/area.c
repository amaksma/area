
typedef double (*func_t)(double);

double area(
    func_t f1, func_t f2, func_t f3,
    func_t df1dx, func_t df2dx, func_t df3dx
    double a1, double b1,
    double a2, double b2,
    double a3, double b3
);
double root(
    func_t f, func_t g,
    func_t dfdx, func_t dgdx,
    double a, double b,
    double eps1
);
double integral(
    func_t f,
    double a, double b,
    double eps2
);

double f1(double x) { return exp(x) + 2; }
double df1dx(double x) { return exp(x); }

enum {
    UNDEFINED,
    FUNC_SIN,
    FUNC_COS,
    FUNC_TAN,
    FUNC_EXP
};

char *derivative(const char *f) {
    
}

int main(void) {
    
    
    return 0;
}