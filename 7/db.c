#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "serialization.h"
#include "text.h"

/*
 * сдвинуть строки, заполнив место удаленной строки
 * db->size уменьшается на 1
 */
static void moveRows(struct database* db, size_t idx)
{
    free(db->rows[idx]);

    for (size_t i = idx; i < db->size - 1; i++) {
        db->rows[i] = db->rows[i + 1];
    }

    db->size--;
}

/*
 * печатает строку 
 */
static void printRow(struct row_t* row, uint8_t hightlight, uint64_t idx)
{
    if (hightlight) {
        printf(FG_GREEN);
    }

    printf("%6lu%20s%20s%20lu%20lu%20lu\n",
           idx,
           row->name,
           row->club,
           row->age,
           row->matches,
           row->goals);

    printf(RESET);
}

struct database* DB_Create()
{
    struct database* db = malloc(sizeof(struct database));

    db->capacity = 10;
    db->size     = 0;

    db->rows = malloc(sizeof(struct row_t*) * db->capacity);

    printf(BOLD INF "База данных успешно создана\n" RESET);

    return db;
}

void DB_Close(struct database* db)
{
    for (size_t i = 0; i < db->size; i++) {
        free(db->rows[i]);
    }

    fclose(db->file);

    free(db);

    printf(BOLD INF "База данных успешно закрыта\n" RESET);
}

void DB_Print(struct database* db)
{
    if (db->size == 0) {
        printf(BG_RED FG_BLACK "База данных пуста\n" RESET);
        return;
    }

    printf(BOLD "%6s%20s%20s%20s%20s%20s\n" RESET,
           "Number",
           "Full name",
           "Club",
           "Age",
           "Matches",
           "Goals");
    for (size_t i = 0; i < db->size; i++) {
        printRow(db->rows[i], 0, i);
    }
    printf("\n");
}

struct database* DB_Open(const char* file)
{
    FILE* f = fopen(file, "rb");

    if (f == NULL) {
        printf(BOLD INF "Файл не существует\n" RESET);
        return NULL;
    }

    struct database* db = DB_Create();

    deserialize(f, db);

    rename(file, ".backup.db");

    fclose(f);

    db->file = fopen(file, "wb");

    printf(BOLD INF "База данных успешно открыта из файла [%s]\n" RESET, file);

    return db;
}

void DB_InsertRow(struct database* db,
                  const char*      name,
                  const char*      club,
                  uint64_t         age,
                  uint64_t         matches,
                  uint64_t         goals)
{
    if (db->size == db->capacity) {
        db->capacity *= 2;
        db->rows = realloc(db->rows, db->capacity * sizeof(struct row_t*));
    }

    db->rows[db->size++] = DB_CreateRow(name, club, age, matches, goals);
}

uint8_t DB_RemoveRow(struct database* db, uint64_t i)
{
    if (i > db->size) {
        return DB_ERROR;
    }
    moveRows(db, i);

    // switch (attr) {
    //     case DB_ATTR_NAME:
    //     {
    //         const char* str = db->rows[i]->name;
    //         if (!strcmp(str, (const char*)value)) {
    //             moveRows(db, i--);
    //         }
    //
    //         break;
    //     }
    //
    //     case DB_ATTR_CLUB:
    //     {
    //         const char* str = db->rows[i]->club;
    //         if (!strcmp(str, (const char*)value)) {
    //             moveRows(db, i--);
    //         }
    //
    //         break;
    //     }
    //
    //     case DB_ATTR_AGE:
    //     {
    //         const uint64_t* age = (uint64_t*)value;
    //         if (db->rows[i]->age == *age) {
    //             moveRows(db, i--);
    //         }
    //
    //         break;
    //     }
    //
    //     case DB_ATTR_MATCHES:
    //     {
    //         const uint64_t* matches = (uint64_t*)value;
    //         if (db->rows[i]->matches == *matches) {
    //             moveRows(db, i--);
    //         }
    //
    //         break;
    //     }
    //
    //     case DB_ATTR_GOALS:
    //     {
    //         const uint64_t* goals = (uint64_t*)value;
    //         if (db->rows[i]->goals == *goals) {
    //             moveRows(db, i--);
    //         }
    //
    //         break;
    //     }
    //
    //     default:
    //         return DB_ERROR;
    // }

    return DB_OK;
}

uint8_t DB_EditRow(struct database* db,
                   uint64_t         i,
                   const char*      name,
                   const char*      club,
                   uint64_t         age,
                   uint64_t         matches,
                   uint64_t         goals)
{
    if (i > db->size) {
        return DB_ERROR;
    }

    strncpy(db->rows[i]->name, name, 64);
    strncpy(db->rows[i]->club, club, 64);
    db->rows[i]->age     = age;
    db->rows[i]->matches = matches;
    db->rows[i]->goals   = goals;

    return DB_OK;
}

uint8_t DB_SearchWithAttribute(struct database* db, uint8_t attr, const void* value)
{
    // код - лапша, но работает
    for (size_t i = 0; i < db->size; i++) {
        switch (attr) {
            case DB_ATTR_NAME:
            {
                const char* str = (char*)value;
                if (!strcmp(db->rows[i]->name, str)) {
                    printRow(db->rows[i], 1, i);
                }
                break;
            }

            case DB_ATTR_CLUB:
            {
                const char* str = (char*)value;
                if (!strcmp(db->rows[i]->club, str)) {
                    printRow(db->rows[i], 1, i);
                }
                break;
            }

            case DB_ATTR_AGE:
            {
                uint64_t* a = (uint64_t*)value;
                if (*a == db->rows[i]->age) {
                    printRow(db->rows[i], 1, i);
                }
                break;
            }

            case DB_ATTR_MATCHES:
            {
                uint64_t* a = (uint64_t*)value;
                if (*a == db->rows[i]->matches) {
                    printRow(db->rows[i], 1, i);
                }
                break;
            }

            case DB_ATTR_GOALS:
            {
                uint64_t* a = (uint64_t*)value;
                if (*a == db->rows[i]->goals) {
                    printRow(db->rows[i], 1, i);
                }
                break;
            }

            default:
                return DB_ERROR;
        }
    }
    return DB_OK;
}

static int cmpName(const void* a, const void* b)
{
    const struct row_t* pa = *(const struct row_t**)a;
    const struct row_t* pb = *(const struct row_t**)b;

    return strcmp(pa->name, pb->name);
}

static int cmpClub(const void* a, const void* b)
{
    const struct row_t* pa = *(const struct row_t**)a;
    const struct row_t* pb = *(const struct row_t**)b;

    return strcmp(pa->club, pb->club);
}

static int cmpAge(const void* a, const void* b)
{
    const struct row_t* pa = *(const struct row_t**)a;
    const struct row_t* pb = *(const struct row_t**)b;

    return pa->age > pb->age;
}

static int cmpMatches(const void* a, const void* b)
{
    const struct row_t* pa = *(const struct row_t**)a;
    const struct row_t* pb = *(const struct row_t**)b;

    return pa->matches > pb->matches;
}

static int cmpGoals(const void* a, const void* b)
{
    const struct row_t* pa = *(const struct row_t**)a;
    const struct row_t* pb = *(const struct row_t**)b;

    return pa->goals > pb->goals;
}

uint8_t DB_SortWithAttribute(struct database* db, uint8_t attr)
{
    switch (attr) {
        case DB_ATTR_NAME:
            qsort(db->rows, db->size, sizeof(struct row_t*), cmpName);
            break;
        case DB_ATTR_CLUB:
            qsort(db->rows, db->size, sizeof(struct row_t*), cmpClub);
            break;
        case DB_ATTR_AGE:
            qsort(db->rows, db->size, sizeof(struct row_t*), cmpAge);
            break;
        case DB_ATTR_MATCHES:
            qsort(db->rows, db->size, sizeof(struct row_t*), cmpMatches);
            break;
        case DB_ATTR_GOALS:
            qsort(db->rows, db->size, sizeof(struct row_t*), cmpGoals);
            break;
        default:
            return DB_ERROR;
    }

    printf(BOLD INF "База данных успешно отсортирована\n" RESET);

    return DB_OK;
}

void DB_Save(struct database* db)
{
    serialize(db->file, db);

    printf(BOLD INF "База данных успешно записана в файл\n" RESET);
}

void DB_SetFile(struct database* db, const char* file)
{
    db->file = fopen(file, "wb");

    printf(BOLD INF "Файл для записи успешно установлен [%s]\n" RESET, file);
}

void DB_RestoreFile(const char* file)
{
    rename(".backup.db", file);
}
