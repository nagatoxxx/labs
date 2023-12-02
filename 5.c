#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// удалить пробелы в начале и конце строки,
// отсортировать по возрастанию длины строки,
// результат в файл g

/*
 * создает файл, наполняет его, возвращает указатель
 */
static FILE* createFile();

/*
 * выполнение задания, возвращает указатель на файл с результатом
 */
static FILE* solve(FILE* f);

/*
 * сравнивает две строки по длине
 * возвращает strlen(a) > strlen(b)
 */
static int comp(const void* a, const void* b);

int main()
{
    FILE* f = createFile();
    FILE* g = solve(f);

    fclose(g);
    fclose(f);
    return 0;
}

static FILE* createFile()
{
    FILE* f = fopen("f.txt", "w+");

    // заполняет файл случайными строками
    char buf[512];
    strcpy(buf, " stroka11  \n");

    fwrite(buf, 1, strlen(buf), f);

    memset(buf, '\0', 32);
    strcpy(buf, "  hello world 23 \n");

    fwrite(buf, 1, strlen(buf), f);

    memset(buf, '\0', 32);
    strcpy(buf, " 123123 123123123 12313123 ");

    fwrite(buf, 1, strlen(buf), f);
    fseek(f, 0, SEEK_SET);

    return f;
}

static FILE* solve(FILE* f)
{
    FILE* g = fopen("g.txt", "w+");

    // вычислить размер файла
    fseek(f, 0, SEEK_END);
    unsigned long sz = ftell(f);
    rewind(f);

    char* buf = malloc(sz);
    fread(buf, 1, sz, f);

    char res[512][128]; // массив строк
    int  i = 0;         // итератор по res
                        // после - размер массива res

    // разбиваем строку по разделителям, записываем в массив строк
    char* pch = strtok(buf, " \n");
    while (pch != NULL) {
        strncpy(res[i++], pch, 128);
        pch = strtok(NULL, " \n");
    }

    // сортируем массив по возрастанию длин строк
    qsort(res, i, sizeof(*res), comp);

    // записываем в массив
    for (int j = 0; j < i; j++) {
        fwrite(res[j], 1, strlen(res[j]), g);
        fputc('\n', g);
    }

    free(buf);

    return g;
}

static int comp(const void* a, const void* b)
{
    const char* pa = (const char*)a;
    const char* pb = (const char*)b;

    return strlen(pa) > strlen(pb);
}
