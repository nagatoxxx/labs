#include <stdio.h>
#include <stdlib.h>

/*
 * размерность массива в программе: m x n
 */
#define m 3
#define n 3

/*
 * считывает матрицу от пользователя
 */
static void readMatrix(int matrix[m][n]);

/*
 * выводит содержимое матрицы в консоль
 */
static void printMatrix(int matrix[m][n]);

/*
 * находит среднее арифметическое всех элементов матрицы
 */
static double avg(int matrix[m][n]);

/*
 * находит сумму элементов столбца, в котором наибольшее число элементов
 * превышает среднее арифметическое всех элементов матрицы
 */
static int sum(int matrix[m][n]);

int main(int argc, char* argv[])
{
    int matrix[m][n];

    readMatrix(matrix);

    printf("Вы ввели:\n");
    printMatrix(matrix);

    printf("Среднее значение матрицы: %.2lf\n", avg(matrix));

    printf("Сумма: %i\n", sum(matrix));

    return 0;
}

static void readMatrix(int matrix[m][n])
{
    for (int i = 0; i < m; i++) {
        printf("Введите строку %i:\n", (i + 1));
        for (int j = 0; j < n; j++) {
            if (scanf("%i", &(matrix[i][j])) != 1) {
                puts("Повторите еще раз!");
                exit(1);
            }
        }
    }
}

static void printMatrix(int matrix[m][n])
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%i ", matrix[i][j]);
        }
        putchar('\n');
    }
}

static double avg(int matrix[m][n])
{
    int sum = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j];
        }
    }

    return (double)sum / (n * m);
}

static int sum(int matrix[m][n])
{
    struct col
    {
        int idx;   // индекс столбца
        int count; // число элементов, превышающих среднее арифметическое
        int sum;   // сумма элементов столбца
    };

    struct col max = {.idx = 0, .count = 0, .sum = 0};

    double average = avg(matrix);

    for (int i = 0; i < m; i++) {
        struct col c = {.idx = i, .count = 0, .sum = 0};

        for (int j = 0; j < n; j++) {
            if (matrix[j][i] > average) {
                c.count++;
            }
            c.sum += matrix[j][i];
        }

        if (c.count > max.count) {
            max = c;
        }
    }

    return max.sum;
}
