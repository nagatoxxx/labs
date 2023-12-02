#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void fillFile(FILE* file); // заполняет файл 10 случайными числами
static void printFile(FILE* file); // выводит содержимое файла
static void solve(FILE* file); // решает задание, записывает результат в файл
/*
 * помещает элемент с индексом a перед элементом с иднексом b в массиве array
 */
static void moveElement(uint8_t* array, int n, int a, int b);

int main(int argc, char* argv[])
{
    FILE* f = fopen("f.bin", "w+b");

    fillFile(f);
    printFile(f);
    solve(f);
    printFile(f);

    fclose(f);

    return 0;
}

static void fillFile(FILE* file)
{
    if (file == NULL) {
        printf("Невозможно открыть файл\n");
        exit(1);
    }
    srand(time(NULL));
    uint8_t n = 0;
    for (int i = 0; i < 10; i++) {
        n = rand() % 20;
        fwrite(&n, sizeof(uint8_t), 1, file);
    }
    fseek(file, 0, SEEK_SET);
}

static void printFile(FILE* file)
{
    uint8_t n = 0;
    for (int i = 0; i < 10; i++) {
        fread(&n, sizeof(uint8_t), 1, file);
        printf("%3i ", n);
    }
    putc('\n', stdout);
    fseek(file, 0, SEEK_SET);
}

static void solve(FILE* file)
{
    uint8_t buf[10];
    fread(buf, sizeof(uint8_t), 10, file);

    uint8_t max     = 0;
    size_t  max_idx = -1;
    uint8_t min     = UINT8_MAX;
    size_t  min_idx = -1;

    for (int i = 0; i < 10; i++) {
        if (buf[i] > max) {
            max     = buf[i];
            max_idx = i;
        }
        else if (buf[i] < min) {
            min     = buf[i];
            min_idx = i;
        }
    }

    moveElement(buf, 10, min_idx, max_idx);
    fseek(file, 0, SEEK_SET);

    fwrite(buf, sizeof(uint8_t), 10, file);

    fseek(file, 0, SEEK_SET);
}

static void moveElement(uint8_t* array, int n, int a, int b)
{
    if (a - b > 0) {
        uint8_t tmp = array[a];
        memmove(&array[b + 1], &array[b], (a - b) * sizeof(uint8_t));
        array[b] = tmp;
    }
    else if (b - a > 1) {
        uint8_t tmp  = array[a];
        array[a]     = array[b - 1];
        array[b - 1] = tmp;
    }
}
