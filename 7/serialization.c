#include "serialization.h"
#include "row.h"

void serialize(FILE* file, const struct database* db)
{
    fseek(file, SEEK_SET, 0);
    fwrite(&db->size, sizeof(uint64_t), 1, file);
    fwrite(&db->capacity, sizeof(uint64_t), 1, file);

    for (size_t i = 0; i < db->size; i++) {
        fwrite(db->rows[i], sizeof(struct row_t), 1, file);
    }
}

void deserialize(FILE* file, struct database* db)
{
    uint64_t size     = 0;
    uint64_t capacity = 0;

    fread(&size, sizeof(uint64_t), 1, file);
    fread(&capacity, sizeof(uint64_t), 1, file);

    struct row_t row;

    for (size_t i = 0; i < size; i++) {
        fread(&row, sizeof(struct row_t), 1, file);
        DB_InsertRow(db, row.name, row.club, row.age, row.goals, row.matches);
    }
}
