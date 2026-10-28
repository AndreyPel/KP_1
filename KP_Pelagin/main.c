#define _CRT_SECURE_NO_WARNINGS
#define M_PI 3.14159265358979323846
#include <stdio.h>
#include <locale.h>
#include <math.h>

double Q(double x);
int check_control(double q);

int main() {
    setlocale(LC_ALL, "RUS");
    double x;
    double q_val;
    int status;
    printf("Введите X: ");
    scanf("%lf", &x);
    q_val = Q(x);
    printf("Q(%.4lf) = %.4lf\n", x, q_val);
    status = check_control(q_val);
    if (status == 1) {
        printf("Состояние: Норма\n");
    }
    else {
        printf("Состояние: Отклонение\n");
    }
    return 0;
}

double Q(double x) {
    if (x < 2) {
        return 2 * (x - 2) + 10;
    }
    else if (x >= 2 && x <= 8) {
        return pow(x - 5, 2) / 1 + 45;
    }
    else {
        return 3 * sqrt(x - 8 + 1) + 55;
    }
}

int check_control(double q) {
    double Q0 = 45.00;
    double dQmax = 11.0;
    double lower_bound = Q0 - dQmax;
    double upper_bound = Q0 + dQmax;
    printf("Допустимый диапазон: [%.2lf; %.2lf]\n", lower_bound, upper_bound);
    if (q >= lower_bound && q <= upper_bound) {
        return 1;
    }
    else {
        return 0;
    }
}
