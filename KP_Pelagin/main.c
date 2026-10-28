#define _CRT_SECURE_NO_DEPRECATE 
#include <stdio.h>
#include <locale.h>
#include <math.h>

// --- Константы для валидации входных данных ---
#define X_MIN 0.0
#define X_MAX 20.0

// --- Границы интервалов для кусочной функции ---
#define X_BOUND_1 2.0
#define X_BOUND_2 8.0

// --- Параметры для контроля состояния ---
#define Q0 45.0
#define DQ_MAX 11.0

// Ветка 1: 2 * (x - 2) + 10
#define F1_MUL 2.0
#define F1_ADD 10.0

// Ветка 2: (x - 5)^2 + 45
#define F2_CENTER 5.0

// Ветка 3: 3 * sqrt(x - 7) + 55
#define F3_MUL 3.0
#define F3_SHIFT 7.0
#define F3_ADD 55.0

double compute_q(double x);
int check_state(double q);

int main() {
    setlocale(LC_ALL, "RUS");

    printf("==================================================\n");
    printf("  Система мониторинга концентрации CO2\n");
    printf("==================================================\n");
    printf("Программа предназначена для мониторинга технологического\n");
    printf("процесса в химическом реакторе.\n");
    printf("Введите значение X в процентах (диапазон: %.0f..%.0f).\n", X_MIN, X_MAX);
    printf("Программа рассчитает нормированный показатель Q и состояние.\n");
    printf("==================================================\n\n");

    double x;
    printf("Введите концентрацию CO2 (X): ");
    scanf("%lf", &x);

    double q_val = compute_q(x);

    printf("\n--- Результаты ---\n");
    printf("X = %.4lf\n", x);
    printf("Q(%.4lf) = %.4lf\n", x, q_val);

    double lower_bound = Q0 - DQ_MAX;
    double upper_bound = Q0 + DQ_MAX;
    printf("Допустимый диапазон Q: [%.2lf; %.2lf]\n", lower_bound, upper_bound);

    int status = check_state(q_val);
    if (status == 1) {
        printf("Состояние: Норма\n");
    }
    else {
        printf("Состояние: Отклонение\n");
    }

    return 0;
}

// Функция расчёта нормированного показателя Q
double compute_q(double x) {
    if (x < X_BOUND_1) {
        return F1_MUL * (x - X_BOUND_1) + F1_ADD;
    }
    else if (x <= X_BOUND_2) {
        return (x - F2_CENTER) * (x - F2_CENTER) + Q0;
    }
    else {
        return F3_MUL * sqrt(x - F3_SHIFT) + F3_ADD;
    }
}

// Функция проверки состояния
int check_state(double q) {
    double lower_bound = Q0 - DQ_MAX;
    double upper_bound = Q0 + DQ_MAX;

    if (q >= lower_bound && q <= upper_bound) {
        return 1;
    }
    else {
        return 0; 
    }
}
