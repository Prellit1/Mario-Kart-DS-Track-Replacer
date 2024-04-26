#include "carc.h"

int write_temp_header(FILE *file)
{
    int a = 0xFFFE;
    fwrite("NARC", 4, sizeof(char), file);
    fwrite(&a, 2, sizeof(char), file);
    a = 0x0100;
    fwrite(&a, 2, sizeof(char), file);
    a = 0xFFFFFFFF;
    fwrite(&a, 4, sizeof(char), file);
    a = 0x10;
    fwrite(&a, 2, sizeof(char), file);
    a = 0x3;
    fwrite(&a, 2, sizeof(char), file);
    return 0x10;
}