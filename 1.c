#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * считает арксинус с заданным числом членов суммы
 * x - число
 * n - число членов суммы
 */
static double arcsin_n(const double x, const int n);

/*
 * считает арксинус с заданной точностью
 * x - число
 * p - точность
 */
static double arcsin_p(const double x, double p);

int main(int argc, char* argv[])
{
    int ch; // выбор пользователя
    printf("Выберите режим:\n1. Число членов суммы;\n2. Заданная "
           "точность.\n> ");
    if (scanf("%d", &ch) != 1) {
        printf("Повторите еще раз.\n");
        return 1;
    }
    switch (ch) {
        case 1:
        {
            int    n = 0;
            double x = 0.0f;
            printf("Введите x и число членов последовательности: ");
            if (scanf("%lf %d", &x, &n) != 2) {
                printf("Ошибка! Повторите еще раз.\n");
                return 1;
            }
            if (abs((int)x) >= 1) {
                printf("|x| < 1!\n");
                return 1;
            }
            printf("Результат: %lf\n", arcsin_n(x, n));
            break;
        }
        case 2:
        {
            double p = 0;
            double x = 0.0f;
            printf("Введите x и точность: ");
            if (scanf("%lf %lf", &x, &p) != 2) {
                printf("Ошибка! Повторите еще раз.\n");
                return 1;
            }

            if (abs((int)x) >= 1) {
                printf("|x| < 1!\n");
                return 1;
            }
            printf("Результат: %lf\n", arcsin_p(x, p));
            break;
        }
        default:
        {
            printf("Введите 1 или 2.\n");
            return 1;
        }
    }


    return 0;
}

static double arcsin_n(const double x, const int n)
{
    double res = x;

    for (int i = 1; i <= n; i++) {
        int num = 1, denum = 1; // числитель и знаменатель
        for (int j = 1; j <= 2 * i - 1; j += 2) {
            num *= j;
        }
        for (int j = 2; j <= 2 * i; j += 2) {
            denum *= j;
        }

        res += (num * pow(x, 2 * i + 1) / denum * (2 * i + 1));
    }

    return res;
}

static double arcsin_p(const double x, double p)
{
    double res = x;
    double t = 10000.0f; // переменная, содержащая последний посчитанный член
    int i = 1;

    while (t >= p) {
        int num = 1, denum = 1;
        for (int j = 1; j <= 2 * i - 1; j += 2) {
            num *= j;
        }
        for (int j = 2; j <= 2 * i; j += 2) {
            denum *= j;
        }
        t = (num * pow(x, 2 * i + 1) / denum * (2 * i + 1));
        res += t;

        i++;
    }

    return res;
}
