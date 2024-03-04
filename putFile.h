#include "carcPut.h"
#include "FileAdd.h"
#include "common.h"
#pragma once
void putCarcInRom(FILE *Carc, FILE *Mkds, int Id, char isTex);
void _fileSizeChange(FILE *Mkds, unsigned int sizeAdded);
void putFileInRom(FILE *Carc, FILE *Mkds, ENTRY_FAT entry);
void putDataInRom(unsigned int dataSize, void *pData, FILE *Mkds, ENTRY_FAT entry);
