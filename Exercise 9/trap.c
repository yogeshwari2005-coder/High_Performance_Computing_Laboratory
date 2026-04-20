#include <stdio.h>
#include <omp.h>

// Function to integrate: f(x) = x^2
double f(double x) {
    return x * x;
}

int main() {
    int n, i;
    double a, b, h;
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    double result1, result2, result3;

    // Input values
    printf("Enter lower limit (a): ");
    scanf("%lf", &a);

    printf("Enter upper limit (b): ");
    scanf("%lf", &b);

    printf("Enter number of trapezoids (n): ");
    scanf("%d", &n);

    h = (b - a) / n;

    // =========================================
    // 1. Using parallel for (without atomic)
    // =========================================
    #pragma omp parallel
    {
        double local_sum1 = 0.0;

        #pragma omp for
        for (i = 1; i < n; i++) {
            double x = a + i * h;
            local_sum1 += f(x);
        }

        #pragma omp critical
        {
            sum1 += local_sum1;
        }
    }

    result1 = h * ((f(a) + f(b)) / 2.0 + sum1);

    // =========================================
    // 2. Using reduction
    // =========================================
    #pragma omp parallel for reduction(+:sum2)
    for (i = 1; i < n; i++) {
        double x = a + i * h;
        sum2 += f(x);
    }

    result2 = h * ((f(a) + f(b)) / 2.0 + sum2);

    // =========================================
    // 3. Using critical
    // =========================================
    #pragma omp parallel
    {
        double local_sum3 = 0.0;

        #pragma omp for
        for (i = 1; i < n; i++) {
            double x = a + i * h;
            local_sum3 += f(x);
        }

        #pragma omp critical
        {
            sum3 += local_sum3;
        }
    }

    result3 = h * ((f(a) + f(b)) / 2.0 + sum3);

    // =========================================
    // Output results
    // =========================================
    printf("\nResults:\n");
    printf("Using parallel for (no atomic) : %f\n", result1);
    printf("Using reduction                : %f\n", result2);
    printf("Using critical                : %f\n", result3);

    return 0;
}
