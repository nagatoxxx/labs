#include <stdlib.h>
#include <string.h>

#include "row.h"

struct row_t* DB_CreateRow(const char* full_name,
                           const char* club_name,
                           uint64_t    age,
                           uint64_t    matches,
                           uint64_t    goals)
{
    struct row_t* row = malloc(sizeof(struct row_t));

    strncpy(row->name, full_name, 64);
    strncpy(row->club, club_name, 64);
    row->age     = age;
    row->matches = matches;
    row->goals   = goals;

    return row;
}
