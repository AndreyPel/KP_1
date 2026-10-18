#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <math.h>

double Q(double X) {
    if (X < 2) {
        return 2 * (X - 2) + 10;
    }
    else if (X <= 8) {
        return (X - 5) * (X - 5) + 45;
    }
    else {
        return 3 * sqrt(X - 8 + 1) + 55;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    double X;
    printf("¬ведите X: ");
    scanf("%lf", &X);

    printf("Q(%.2f) = %.4f\n", X, Q(X));

    return 0;
}