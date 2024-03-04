#include "ARM9.h"
#include <string.h>

unsigned int leng;

unsigned int getArm9Addr(FILE *Mkds)
{
    fseek(Mkds, 0x20, SEEK_SET);
    int result;
    fread(&result, sizeof(int), 1, Mkds);
    return result;
}

unsigned int getArm9Size(FILE *Mkds)
{
    fseek(Mkds, 0x2C, SEEK_SET);
    int result;
    fread(&result, sizeof(int), 1, Mkds);
    return result;
}

NFooter getArm9Footer(FILE *Mkds)
{
    int addr = getArm9Addr(Mkds);
    int size = getArm9Size(Mkds);

    fseek(Mkds, addr + size, SEEK_SET);
    NFooter result;
    fread(&result, sizeof(NFooter), 1, Mkds);
    if (result.NCode != 0xDEC00621)
        result.NCode = -1; // lol
    return result;
}

char *getArm9InAlloc(FILE *Mkds)
{
    int len = getArm9Size(Mkds);
    char *arm9 = (char *)calloc(len, 1);

    int addr = getArm9Addr(Mkds);
    fseek(Mkds, addr, SEEK_SET);

    fread(arm9, len, 1, Mkds);
    return arm9;
}

unsigned char *COPIED_MIi_UncompressBackward(unsigned char *Data, unsigned int Length)
{

    leng = *(unsigned int *)&Data[Length - 4] + Length;

    char *Result = (char *)calloc(leng, 1);
    /* Array.Copy(Data, Result, Length); */
    memcpy(Result, Data, Length);
    int Offs = (int)(Length - ((*(unsigned int *)&Data[Length - 8]) >> 24));
    int dstoffs = (int)leng;

    while (1)
    {

        char header = Result[--Offs];
        for (int i = 0; i < 8; i++)
        {
            if (!(header & 0x80))
            {

                Result[--dstoffs] = Result[--Offs];
            }
            else
            {

                unsigned char a = Result[--Offs];
                unsigned char b = Result[--Offs];
                // char a = b;
                unsigned int offse = (((a & 0xF) << 8) | b) + 2;
                int length = (a >> 4) + 2;
                do
                {
                    Result[dstoffs - 1] = Result[dstoffs + offse];
                    dstoffs--;
                    length--;
                } while (length >= 0);
            }
            if (Offs <= (Length - (*(unsigned int *)&Data[Length - 8] & 0xffffff)))
                return Result;
            header = header << 1;
        }
    }
}
void makeBE(char *buf, int size)
{
    for (int i = 0; i < size; i = i + 2)
    {
        char temp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = temp;
    }
}
char *COPIED_Decompress(char *Data, unsigned int _start_ModuleParamsOffset, int Length)
{
    if (*(unsigned int *)&Data[_start_ModuleParamsOffset + 0x14] == 0)
    {
        leng = Length;
        return Data; // Not Compressed!
    }

    char *Result;

    Result = COPIED_MIi_UncompressBackward(Data, Length);

    *(unsigned int *)&Result[_start_ModuleParamsOffset + 0x14] = 0;
    /* IOUtil.WriteU32LE(Result, (int)_start_ModuleParamsOffset + 0x14, 0); */

    return Result;
}

/* int main()
{
    FILE *mkds = fopen("EMKDSEU.nds", "rb");
    // int addr = getArm9Addr(mkds);
    printf("a");
    int size = getArm9Size(mkds);
    NFooter NF = getArm9Footer(mkds);
    printf("b");
    char *arm9 = getArm9InAlloc(mkds);

    FILE *arm0 = fopen("S0.bin", "wb");

    // printf("%d", leng);

    fwrite(arm9, size, 1, arm0);

    printf("\n SIZ : %d\n", size);
    char *decompArm9 = COPIED_Decompress(arm9, NF.OffsetToArm9Addr, size);
    printf("d");

    FILE *arm = fopen("S.bin", "wb");
    printf("%d", leng);
    fwrite(decompArm9, leng, 1, arm);

    fclose(arm);
    fclose(arm0);
    fclose(mkds);
    free(arm9);
    free(decompArm9);
    return 0;
} */