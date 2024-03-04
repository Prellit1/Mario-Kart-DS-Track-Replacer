#include "../common.h"

#pragma once
typedef struct
{
    unsigned int NCode;
    unsigned int OffsetToArm9Addr;
    unsigned int unk;
} NFooter;
char *COPIED_Decompress(char *Data, unsigned int _start_ModuleParamsOffset, int Length);
uint8_t *COPIED_MIi_UncompressBackward(uint8_t *Data, uint32_t Length);
NFooter getArm9Footer(FILE *mkds);
extern unsigned int leng;
unsigned int getArm9Addr(FILE *Mkds);
unsigned int getArm9Size(FILE *Mkds);
NFooter getArm9Footer(FILE *Mkds);
char *getArm9InAlloc(FILE *Mkds);
