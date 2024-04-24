#include "carc.h"

int write_img(FILE *f, FILE *Wrote)
{
    fseek(Wrote, 0, SEEK_END);
    int size = ftell(Wrote) + 8;
    fwrite("GMIF", 4, 1, f);
    fwrite(&size, 4, 1, f);
    transmit_file(f, Wrote);
    return size;
}