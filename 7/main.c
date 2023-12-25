#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "text.h"

struct database* db = NULL;

/*
 * считывает произвольные данные
 */
#define read(buffer, size, format, ...)           \
    do {                                          \
        fgets(buffer, size, stdin);               \
        size_t len = strlen(buffer);              \
        if (len > 0 && buffer[len - 1] == '\n') { \
            buffer[len - 1] = '\0';               \
        }                                         \
        sscanf(buffer, format, __VA_ARGS__);      \
    } while (0);

/*
 * считывает строку
 */
#define readString(buffer, size)                  \
    do {                                          \
        fgets(buffer, size, stdin);               \
        size_t len = strlen(buffer);              \
        if (len > 0 && buffer[len - 1] == '\n') { \
            buffer[len - 1] = '\0';               \
        }                                         \
    } while (0)


int main()
{
    char buf[512]; // буфер для пользовательского ввода

    printf(BOLD "Выберите действие:" RESET "\n1. Создать новую базу данных"
                "\n2. Открыть существующую базу данных"
                "\n---> ");

    int i = 0;

    read(buf, sizeof(buf), "%i", &i);

    char filename[253];

    switch (i) {
        case 1:
            printf(BOLD "Введите имя файла: " RESET);
            read(buf, sizeof(buf), "%s", filename);
            strcat(filename, ".db");
            db = DB_Create();
            DB_SetFile(db, filename);
            break;
        case 2:
            printf(BOLD "Введите имя файла: " RESET);
            read(buf, sizeof(buf), "%s", filename);
            strcat(filename, ".db");
            db = DB_Open(filename);
            break;
        default:
            printf(ERR "Некорректный ввод\n" RESET);
            return 1;
    }

    if (db == NULL) {
        printf(ERR "Произошла ошибка!\n" RESET);
        return 1;
    }

    while (1) {
        int i; // выбор пользователя
        printf(BOLD "Выберите действие:" RESET "\n1. Добавить запись"
                    "\n2. Удалить запись"
                    "\n3. Найти запись"
                    "\n4. Редактировать запись"
                    "\n5. Сортировать базу данных"
                    "\n6. Вывести содержимое базы данных"
                    "\n"
                    "\n7. Сохранить данные и выйти" BOLD FG_RED "\n8. Выйти "
                    "без "
                    "сохранения" RESET "\n---> ");
        read(buf, sizeof(buf), "%i", &i);

        switch (i) {
            case 1:
            {
                char              n[64], c[64];
                long unsigned int a, m, g;

                printf("Введите данные спортсмена:\n");

                printf("Полное имя: ");
                readString(n, 64);

                printf("Название клуба: ");
                readString(c, 64);

                printf("Возраст: ");
                read(buf, sizeof(buf), "%lu", &a);

                printf("Количество матчей: ");
                read(buf, sizeof(buf), "%lu", &m);

                printf("Количество забитых мячей: ");
                read(buf, sizeof(buf), "%lu", &g);

                DB_InsertRow(db, n, c, a, m, g);

                break;
            }
            case 2:
            {
                printf("Введите номер строки: ");
                int i;
                read(buf, sizeof(buf), "%i", &i);

                DB_RemoveRow(db, i);

                break;
            }
            case 3:
            {
                printf("Введите название столбца (имя, клуб, возраст, матчи, "
                       "голы): ");
                char attr[16];
                readString(attr, 16);

                if (strcmp(attr, "имя") == 0) {
                    char value[64];
                    printf("Введите имя: ");
                    readString(value, 64);

                    DB_SearchWithAttribute(db, DB_ATTR_NAME, value);
                }
                else if (strcmp(attr, "клуб") == 0) {
                    char value[64];
                    printf("Введите название клуба: ");
                    readString(value, 64);

                    DB_SearchWithAttribute(db, DB_ATTR_CLUB, value);
                }
                else if (strcmp(attr, "возраст") == 0) {
                    unsigned long int a;
                    printf("Введите возраст: ");
                    read(buf, sizeof(buf), "%lu", &a);

                    DB_SearchWithAttribute(db, DB_ATTR_AGE, &a);
                }
                else if (strcmp(attr, "матчи") == 0) {
                    unsigned long int m;
                    printf("Введите количество матчей: ");
                    read(buf, sizeof(buf), "%lu", &m);

                    DB_SearchWithAttribute(db, DB_ATTR_MATCHES, &m);
                }
                else if (strcmp(attr, "голы") == 0) {
                    unsigned long int g;
                    printf("Введите количество забитых голов: ");
                    read(buf, sizeof(buf), "%lu", &g);

                    DB_SearchWithAttribute(db, DB_ATTR_GOALS, &g);
                }
                else {
                    printf(ERR "Повторите ввод!\n" RESET);
                }
                break;
            }
            case 4:
            {
                printf("Введите номер строки: ");
                int i;
                read(buf, sizeof(buf), "%i", &i);

                char              n[64], c[64];
                long unsigned int a, m, g;


                printf("Введите данные спортсмена:\n");

                printf("Полное имя: ");
                readString(n, 64);

                printf("Название клуба: ");
                readString(c, 64);

                printf("Возраст: ");
                read(buf, sizeof(buf), "%lu", &a);

                printf("Количество матчей: ");
                read(buf, sizeof(buf), "%lu", &m);

                printf("Количество забитых мячей: ");
                read(buf, sizeof(buf), "%lu", &g);

                DB_EditRow(db, i, n, c, a, m, g);

                break;
            }
            case 5:
            {
                printf("Введите название столбца (имя, клуб, возраст, матчи, "
                       "голы): ");
                char attr[16];
                readString(attr, 16);

                if (strcmp(attr, "имя") == 0) {
                    DB_SortWithAttribute(db, DB_ATTR_NAME);
                }
                else if (strcmp(attr, "клуб") == 0) {
                    DB_SortWithAttribute(db, DB_ATTR_CLUB);
                }
                else if (strcmp(attr, "возраст") == 0) {
                    DB_SortWithAttribute(db, DB_ATTR_AGE);
                }
                else if (strcmp(attr, "матчи") == 0) {
                    DB_SortWithAttribute(db, DB_ATTR_MATCHES);
                }
                else if (strcmp(attr, "голы") == 0) {
                    DB_SortWithAttribute(db, DB_ATTR_GOALS);
                }
                else {
                    printf(ERR "Повторите ввод!\n" RESET);
                }
                break;
            }
            case 6:
            {
                DB_Print(db);
                break;
            }
            case 7:
            {
                DB_Save(db);
                goto exit;
            }
            case 8:
            {
                DB_RestoreFile(filename);
                goto exit;
            }
            default:
                printf(ERR "Некорректный ввод\n" RESET);
                return 1;
        }
    }

exit:
    DB_Close(db);
    return 0;
}
