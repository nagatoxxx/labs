#ifndef ROW_H
#define ROW_H

#include <stdint.h>

struct row_t
{
    char     name[64];
    char     club[64];
    uint64_t age;
    uint64_t matches;
    uint64_t goals;
};

/*
 * создает запись и возвращает укаазатель
 * использует malloc
 */
struct row_t* DB_CreateRow(const char* name,
                           const char* club,
                           uint64_t    age,
                           uint64_t    matches,
                           uint64_t    goals);

#endif
