#include "carc.h"

int write_fat(FILE *f)
{
    fwrite("BTAF", 4, 1, f);
    int temp = (id_file * 8) + 12;
    fwrite(&temp, 4, 1, f);
    fwrite(&id_file, 2, 1, f);
    temp = 0;
    fwrite(&temp, 2, 1, f);
    for (int num = 0; num < id_file; num++)
    {
        fwrite(&files_start[num], 4, 1, f);
        fwrite(&files_end[num], 4, 1, f);
    }
    return (id_file * 8) + 12;
}