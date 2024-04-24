#include "FileAdd.h"
#include "carcPut.h"

char trackIdConv[] = {
    18,
    51,
    21,
    30,
    30,
    16,
    19,
    19,
    26,
    3,
    5,
    25,
    31,
    2,
    17,
    17,
    24,
    41,
    40,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    7,
    8,
    35,
    38,
    34,
    15,
    22,
    37,
    11,
    36,
    13,
    13,
    1,
    9,
    10,
    10,
    14,
    12,
    39,
    0,
    32,
    28,
    23,
    29,
    33,
    52,
    53,
    20,
    4,
    27,
    6,
};

FATID *allTheFat;
int fatSize;
unsigned int _getFatAddr(FILE *Mkds)
{
    fseek(Mkds, 0x48, SEEK_SET);
    unsigned int fat;
    fread(&fat, sizeof(int), 1, Mkds);
    return fat;
}
void getFat(FILE *mkds)
{

    unsigned int addrF = _getFatAddr(mkds);
    fseek(mkds, 0x4c, SEEK_SET);
    // unsigned int size;
    fread(&fatSize, 4, 1, mkds);
    freeFat();
    allTheFat = malloc(fatSize);

    fseek(mkds, addrF, SEEK_SET);
    fread(allTheFat, fatSize, 1, mkds);
    fseek(mkds, 0, SEEK_SET);
    // printf("%x   -\n%d   +\n", allTheFat[AIRSHIP].addressStart, sizeof(allTheFat));
}
void freeFat()
{
    if (allTheFat)
    {
        free(allTheFat);
        allTheFat = NULL;
    }
}

ENTRY_FAT getCourse(int id)
{
    ENTRY_FAT course;
    course.addressStart = allTheFat[id].addressStart;
    course.size = (allTheFat[id].addressEnd) - (allTheFat[id].addressStart);
    // printf("%u,  %u  | %u  | %u\n", allTheFat[id].addressStart, allTheFat[id].addressEnd, allTheFat[id].addressEnd - allTheFat[id].addressStart, id);
    course.id = id;
    return course;
}

ENTRY_FAT getCourseTex(int idCourse)
{
    ENTRY_FAT course;
    if (idCourse == CROSSnDL || idCourse == BEACHnDL || idCourse == MANSIONnDL || idCourse == OLDLUIGIGCnDL || idCourse == OLDMOMO64nDL)
    {
        idCourse += 2;
    }
    idCourse += 1;
    course = getCourse(idCourse);
    return course;
}
void _readWrite(unsigned int cur, unsigned int offset, FILE *file)
{
    unsigned int temp2 = 0;
    fseek(file, cur, SEEK_SET);
    fread(&temp2, sizeof(int), 1, file);
    fseek(file, cur, SEEK_SET);
    temp2 += offset;
    fwrite(&temp2, sizeof(int), 1, file);
}
void moveAddrRelatToStartAddr(ENTRY_FAT entry, unsigned int newSize, FILE *file)
{
    int temp[2];
    int offset;
    offset = newSize - entry.size;
    /* printf("OFFSET/NWS/ENS : %d %d %d\n", offset, newSize, entry.size); */
    if (entry.id == 0xFF) // == fakeFat -> == arm9
    {
        unsigned int lengt = newSize;

        fseek(file, 0x2c, SEEK_SET);
        fwrite(&lengt, sizeof(int), 1, file);

        _readWrite(0x30, offset, file);

        _readWrite(0x40, offset, file);

        _readWrite(0x48, offset, file);

        _readWrite(0x50, offset, file);

        _readWrite(0x68, offset, file);

        //_readWrite(0x80, offset, file);
    }
    unsigned int fatAddr;
    fseek(file, 0x48, SEEK_SET);
    fread(&fatAddr, sizeof(int), 1, file);
    // va_list lengi;
    // va_start(lengi, file);
    for (int i = 0; i < fatSize; i++)
    {
        if (entry.addressStart < allTheFat[i].addressStart)
        {
            /* printf("Modif %x %x %x\n", allTheFat[i].addressStart, allTheFat[i].addressEnd, entry.addressStart); */
            fseek(file, fatAddr + (i * 8), SEEK_SET);
            // fprintf(file, "%04x%04x", (allTheFat[i].addressStart + course.size), (allTheFat[i].addressEnd + course.size));

            temp[0] = (allTheFat[i].addressStart + offset);
            temp[1] = (allTheFat[i].addressEnd + offset);
            fwrite(temp, 8, 1, file);

            /* printf("%x %x %d  \n", temp[0], temp[1], offset); */
        }
    }

    /*start of: PROB USELESS BUT NOT GONNA TRY TOUCHING IT JUST IN CASE*/
    if (entry.id != 0xff) // !fakeFat -> !arm9
    {
        fseek(file, fatAddr + (entry.id * 8) + 4, SEEK_SET);
        unsigned int temp2 = (entry.addressStart + newSize);

        fwrite(&temp2, sizeof(int), 1, file);
    }
    /* end of : PROB USELESS BUT NOT GONNA TRY TOUCHING IT JUST IN CASE*/
}

char getGameCourseID(int ID)
{

    return trackIdConv[((ID - 35) >> 1)];
}

/* int main()
{
    COURSE_FAT course;
    FILE *file = fopen("EMKDSEU.nds", "rb");
    getFat(file);

    fclose(file);
    return 0;
} */