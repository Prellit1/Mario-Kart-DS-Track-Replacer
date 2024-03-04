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

uint8_t *COPIED_MIi_UncompressBackward(uint8_t *Data, uint32_t Length)
{

    leng = *(uint32_t *)&Data[Length - 4] + Length;

    uint8_t *Result = (uint8_t *)calloc(leng, 1);
    /* Array.Copy(Data, Result, Length); */
    memcpy(Result, Data, Length);
    int32_t Offs = (int32_t)(Length - ((*(uint32_t *)&Data[Length - 8]) >> 24));
    int32_t dstoffs = (int32_t)leng;

    while (1)
    {
        // printf("\ndd");

        uint8_t header = Result[--Offs];
        for (int32_t i = 0; i < 8; i++)
        {
            if (!(header & 0x80))
            {

                Result[--dstoffs] = Result[--Offs];
            }
            else
            {

                uint8_t a = Result[--Offs];
                uint8_t b = Result[--Offs];
                // char a = b;
                int32_t offse = (((a & 0xF) << 8) | b) + 2;
                int32_t length = (a >> 4) + 2;
                do
                {
                    Result[dstoffs - 1] = Result[dstoffs + offse];
                    dstoffs--;
                    length--;
                } while (length >= 0);
            }
            if (Offs <= (Length - (*(uint32_t *)&Data[Length - 8] & 0x00FFFFFF)))
                return Result;
            header = header << 1;
        }
    }
}

char *COPIED_Decompress(char *Data, unsigned int _start_ModuleParamsOffset, int Length)
{
    if (/* IOUtil.ReadU32LE(Data, (int)_start_ModuleParamsOffset + 0x14) */ *(unsigned int *)&Data[_start_ModuleParamsOffset + 0x14] == 0)
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
    FILE *mkds = fopen("EMKDSEU.nds", "rb+");
    // int addr = getArm9Addr(mkds);
    printf("a");
    int size = getArm9Size(mkds);
    NFooter NF = getArm9Footer(mkds);
    printf("b");
    char *arm9 = getArm9InAlloc(mkds);

    // FILE *arm0 = fopen("S0.bin", "wb+");

    // printf("%d", leng);

    // fwrite(arm9, size, 1, arm0);

    printf("c");
    char *decompArm9 = COPIED_Decompress(arm9, NF.OffsetToArm9Addr, size);
    printf("d\n%d\n", decompArm9[0x4004]);

    FILE *arm = fopen("S.bin", "wb+");
    printf("%d", leng);
    fwrite(decompArm9, leng, 1, arm);

    fclose(arm);
    // fclose(arm0);
    fclose(mkds);
    free(arm9);
    free(decompArm9);
    return 0;
} */