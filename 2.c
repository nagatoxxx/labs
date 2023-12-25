#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a > b) ? (a) : (b))

int* array = NULL;

/*
 * выводит содержимое массива в консоль
 */
static void printArray(int* array, int k);

/*
 * считывает массив от пользователя
 */
static void readArray(int* array, int k);

/*
 * возвращает максимальный элемент, удовлетворяющий условию
 */
static int max(int* array, int k, int Y);

/*
 * находит среднее арифметическое элементов, удовлетворяющих условию
 */
static double avg(int* array, int k);

/*
 * чтобы избежать утечки памяти при завершении программы
 */
static void onExit(void);


int main(int argc, char* argv[])
{
    atexit(onExit);

    int k = 0; // размерность массива
    int X = 0, Y = 0;

    printf("Введите k, X, Y через пробел: ");
    if (scanf("%i %i %i", &k, &X, &Y) != 3) {
        printf("Повторите еще раз\n");
        return 1;
    }

    if (!k) {
        printf("Размер массива должен быть больше чем 0!\n");
        return 1;
    }

    array = malloc(k * sizeof(int));

    printf("Введите массив по одному символу:\n");
    readArray(array, k);
    printf("Массив: ");
    printArray(array, k);

    printf("max: %i\n", max(array, k, Y));
    printf("avg: %.2lf\n", avg(array, k));

    return 0;
}

static void printArray(int* array, int k)
{
    for (int i = 0; i < k; i++) {
        printf("%i ", array[i]);
    }
    putc('\n', stdout);

    return;
}

static void readArray(int* array, int k)
{
    for (int i = 0; i < k; i++) {
        scanf("%i", &(array[i]));
    }
}

static int max(int* array, int k, int Y)
{
    int m = 0;
    for (int i = 0; i < k; i++) {
        if (abs(array[i]) < Y) {
            m = MAX(array[i], m);
        }
    }

    return m;
}

static double avg(int* array, int k)
{
    int sum   = 0;
    int count = 0;

    int firstEven = -1; // индекс первого четного элемента
    int lastEven = -1; // индекс последнего четного элемента

    for (int i = 0; i < k; i++) {
        if (firstEven == -1 && !(array[i] & 1)) {
            firstEven = i;
            continue;
        }

        if (!(array[i] & 1)) { // проверка на четность
            lastEven = i;
        }
    }

    if (firstEven == lastEven - 1) {
        return 0.0;
    }

    if (firstEven == -1 || lastEven == -1) {
        return 0.0f;
    }

    for (int i = firstEven + 1; i < lastEven; i++) {
        sum += array[i];
        count += 1;
    }

    if (k == lastEven) {
        sum -= array[lastEven];
        count--;
    }

    return (double)sum / count;
}

static void onExit(void)
{
    free(array);
}
