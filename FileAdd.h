#include "common.h"
#pragma once

unsigned int getFileLength(FILE *a);
void *putFileInPointer(FILE *file);
void *getRestOfFileWithOffsetAlsoSetCursorToAddrOfReplacedData(FILE *file, int addrOfStartOfReplData, int numOfDataReplaced);

int _getRelativeFileLength(FILE *file, int where);
void *_putUrFileOffsetAfterReplacedData(FILE *file, int relLen, int replaceLen);
void *_putUrFileOffsetInAFXXXingPointer(FILE *file, int relLen);