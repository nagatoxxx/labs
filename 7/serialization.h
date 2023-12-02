#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stdio.h>

#include "db.h"

/*
 * записывает поля структуры в файл
 */
void serialize(FILE* file, const struct database* db);

/*
 * считывает структуру из файла
 */
void deserialize(FILE* file, struct database* db);

#endif
