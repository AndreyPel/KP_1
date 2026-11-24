#define _CRT_SECURE_NO_DEPRECATE 
#include <stdio.h>
#include <locale.h>
#include <math.h>

#define X_MIN      0.0
#define X_MAX      20.0
#define X_BOUND_1  2.0
#define X_BOUND_2  8.0
#define Q0         45.0
#define DQ_MAX     11.0

#define MAX_LINE   256
#define MAX_FILE   256
#define DEFAULT_FILE "co2_data.txt"

/* Прототипы */
double compute_q(double x);
int    check_state(double q);
int    read_value(FILE* f, double* x);
void   print_record(int num, double x, double q, int state, int signal);
void   process_file(const char* file_name, int mode);
void   change_settings(char* file_name, int* mode);

int main(void) {
    char file_name[MAX_FILE] = DEFAULT_FILE;
    int  mode = 1;
    int  choice;

    setlocale(LC_ALL, "RUS");

    printf("==================================================\n");
    printf("  Мониторинг реактора: концентрация CO2 (вариант 31)\n");
    printf("==================================================\n");
    printf("Диапазон X: %.0f..%.0f %%. Базовое Q0 = %.2f, допуск dQmax = %.2f.\n",
        X_MIN, X_MAX, Q0, DQ_MAX);
    printf("Контрольный пример: X = 2.00 %% -> ожидается Q = 54.00.\n");
    printf("==================================================\n");

    while (1) {
        printf("\n==================================================\n");
        printf("                   МЕНЮ ПРОГРАММЫ                 \n");
        printf("==================================================\n");
        printf("1 - Обработать файл измерений\n");
        printf("2 - Изменить настройки (имя файла, режим вывода)\n");
        printf("3 - Выход из программы\n");
        printf("==================================================\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nОшибка ввода: необходимо ввести число (1, 2 или 3).\n");
            continue;
        }

        switch (choice) {
        case 1:
            process_file(file_name, mode);
            break;
        case 2:
            change_settings(file_name, &mode);
            break;
        case 3:
            printf("\nПрограмма завершена.\n");
            return 0;
        default:
            printf("\nНеизвестный пункт меню, попробуйте снова.\n");
            break;
        }
    }

    return 0;
}

double compute_q(double x) {
    if (x < X_BOUND_1) {
        return 2.0 * (x - 2.0) + 10.0;
    }
    else if (x <= X_BOUND_2) {
        return (x - 5.0) * (x - 5.0) + 45.0;
    }
    else {
        return 3.0 * sqrt(x - 7.0) + 55.0;
    }
}

int check_state(double q) {
    return (fabs(q - Q0) <= DQ_MAX) ? 1 : 0;
}

int read_value(FILE* f, double* x) {
    char line[MAX_LINE];
    if (fgets(line, sizeof(line), f) == NULL) {
        return 0;
    }
    if (sscanf(line, "%lf", x) != 1) {
        return -1;
    }
    return 1;
}

void print_record(int num, double x, double q, int state, int signal) {
    const char* state_text = state ? "Норма" : "Отклонение";
    const char* control_text = signal ? "СИГНАЛ: резкое изменение Q!" : "OK";

    printf(" %6d | %10.4lf | %10.4lf | %-10s | %s\n",
        num, x, q, state_text, control_text);
}

void process_file(const char* file_name, int mode) {
    FILE* f = fopen(file_name, "r");
    if (!f) {
        printf("\nОшибка: не удалось открыть файл \"%s\".\n", file_name);
        printf("Проверьте, что файл существует и лежит в папке с программой.\n");
        return;
    }

    int    total = 0, errors = 0, deviations = 0, signals = 0;
    int    have_prev = 0;
    double q_prev = 0.0;

    printf("\n--- Обработка файла \"%s\" ---\n", file_name);
    printf(" Запись |     X      |     Q      | Состояние  | Контроль\n");
    printf("--------+------------+------------+------------+---------------------------\n");

    double x;
    int status;
    while ((status = read_value(f, &x)) != 0) {
        total++;

        if (status == -1) {
            printf(" %6d | некорректная запись, пропущена\n", total);
            errors++;
            continue;
        }

        if (x < X_MIN || x > X_MAX) {
            printf(" %6d | %10.2lf - значение вне диапазона, пропущено\n", total, x);
            errors++;
            continue;
        }

        double q = compute_q(x);
        int state = check_state(q);

        int signal = have_prev && (fabs(q_prev - q) > DQ_MAX) ? 1 : 0;

        have_prev = 1;
        q_prev = q;

        if (!state) deviations++;
        if (signal) signals++;

        if (mode == 1 || !state || signal) {
            print_record(total, x, q, state, signal);
        }
    }

    fclose(f);

    if (total == 0) {
        printf("\nФайл пуст: записи не найдены.\n");
    }
    else {
        printf("\nКонец файла. Всего строк: %d, обработано: %d, "
            "ошибок: %d, отклонений: %d, сигналов: %d.\n",
            total, total - errors, errors, deviations, signals);
    }
}

static void copy_string(char* dst, const char* src, int max_len) {
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static int equals_zero(const char* s) {
    return (s[0] == '0' && s[1] == '\0');
}

void change_settings(char* file_name, int* mode) {
    char tmp[MAX_FILE];
    int  m;

    printf("\n--- Настройки ---\n");
    printf("Текущий файл : %s\n", file_name);
    printf("Текущий режим: %s\n", *mode == 1 ? "подробный" : "только отклонения");

    printf("\nВведите новое имя файла (или 0, чтобы оставить без изменений): ");
    if (scanf("%255s", tmp) == 1) {
        if (!equals_zero(tmp)) {
            copy_string(file_name, tmp, MAX_FILE);
        }
    }

    printf("Введите режим вывода (1 - подробный, 2 - только отклонения): ");
    if (scanf("%d", &m) == 1 && (m == 1 || m == 2)) {
        *mode = m;
    }
    else {
        printf("Некорректный ввод режима, оставлено прежнее значение.\n");
    }
    printf("Настройки успешно применены.\n");
}
