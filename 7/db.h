#ifndef DB_H
#define DB_H

#include <stdint.h>
#include <stdio.h>

#include "row.h"

#define DB_OK    0
#define DB_ERROR 1

#define DB_ATTR_NAME    1 << 0
#define DB_ATTR_CLUB    1 << 1
#define DB_ATTR_AGE     1 << 2
#define DB_ATTR_MATCHES 1 << 3
#define DB_ATTR_GOALS   1 << 4

struct database
{
    /*
     * работает как std::vector
     * сам расширяется и реаллоцирует память при надобности
     *
     * rows аллоцируется malloc
     */
    uint64_t capacity; // вместительность
    uint64_t size;     // текущее количество элементов

    struct row_t** rows;

    FILE* file;
};

/*
 * создает пустую базу данных
 * резервирует res мест для строк базы
 */
struct database* DB_Create();

/*
 * освобождает место, выделенное для db,
 * уничтожает все записи
 */
void DB_Close(struct database* db);

/*
 * выводит полное содержимое базы данных
 */
void DB_Print(struct database* db);

/*
 * открывает существующую базу данных из файла
 */
struct database* DB_Open(const char* file);

/*
 * создает запись в базе данных
 */
void DB_InsertRow(struct database* db,
                  const char*      name,
                  const char*      club,
                  uint64_t         age,
                  uint64_t         matches,
                  uint64_t         goals);

/*
 * удаляет i-ю строку
 * возвращаемые значения: DB_OK, RB_ERROR
 */
uint8_t DB_RemoveRow(struct database* db, uint64_t i);

/*
 * редактирует i строку
 * возможные атрибуты: DB_ATTR_*
 * возвращаемые значения: DB_OK, DB_ERROR
 */
uint8_t DB_EditRow(struct database* db,
                   uint64_t         i,
                   const char*      name,
                   const char*      club,
                   uint64_t         age,
                   uint64_t         matches,
                   uint64_t         goals);

/*
 * выводит в консоль результат поиска
 * возможные значения mode: DB_SEARCH_MODE_*
 * возвращаемые значения: DB_OK, DB_ERROR
 */
uint8_t DB_SearchWithAttribute(struct database* db, uint8_t attr, const void* value);

/*
 * сортирует базу данных по заданному атрибуту DB_ATTR_*
 * НЕ ВЫВОДИТ РЕЗУЛЬТАТ
 * возвращаемые значения: DB_OK, DB_ERROR
 */
uint8_t DB_SortWithAttribute(struct database* db, uint8_t attr);

/*
 * сохраняет текущее состояние базы данных и записывает его в файл
 */
void DB_Save(struct database* db);
/*
 * устанавливает файл для сохранения
 */
void DB_SetFile(struct database* db, const char* file);

/*
 * в случае выхода без сохранения восстанавливает исходныц файл без изменений
 */
void DB_RestoreFile(const char* file);

#endif
