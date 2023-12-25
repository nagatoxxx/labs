#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// печатает все слова, отличные от предыдущего, преобразовывая выводимые слова
static void print(const char* str);
// возвращает указатель на первый символ последнего слова
static char* getLastWord(char* str);
// удаляет из строки все предыдущие вхождения последней буквы, печатает слово
static void removeChar(char* str);

int main()
{
    char buf[512];
    printf("-> Введите текст:\n");
    fgets(buf, 512, stdin);
    if (strlen(buf) == 0 || buf[0] == '.') {
        printf("Строка должна содержать как минимум 1 слово\n");
        return 1;
    }
    buf[strlen(buf) - 1] = '\0';
    if (buf[strlen(buf) - 1] != '.') {
        printf("Строка должна оканчиваться точкой\n");
        return 1;
    }
    printf("-> Преобразованная строка:\n");

    print(buf);

    printf("-> Вы ввели:\n%s\n", buf);


    return 0;
}

static void print(const char* str)
{
    char* buf            = strdup(str);
    buf[strlen(buf) - 1] = '\0';

    char* lastWord = getLastWord(buf);

    char* token = strtok(buf, " ");
    while (token != NULL) {
        if (strcmp(token, lastWord) != 0) {
            removeChar(token);
        }
        token = strtok(NULL, " ");
    }

    free(buf);
}

static char* getLastWord(char* str)
{
    if (str == NULL) {
        return NULL;
    }
    char* p = strrchr(str, ' ');
    if (p == NULL) {
        return str;
    }
    if (p && *(p + 1))
        return (p + 1);
    else
        return NULL;
}

static void removeChar(char* str)
{
    char last = str[strlen(str) - 1];

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != last)
            printf("%c", str[i]);
    }
    printf("%c\n", last);
}
