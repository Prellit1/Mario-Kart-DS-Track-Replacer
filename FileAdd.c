#include "FileAdd.h"

int getFileLength(FILE *a)
{ // gets the file length after the offset (prob with cursor func)

    fseek(a, 0, SEEK_END);
    int result = ftell(a);
    fseek(a, 0, SEEK_SET);

    return result;
}
int _getRelativeFileLength(FILE *file, int where)
{ // gets the size but after the {where}

    // int where = ftell(file);

    int length = getFileLength(file);
    fseek(file, where, SEEK_SET);

    return length - where;
}
void *_putUrFileOffsetInAFXXXingPointer(FILE *file, int relLen)
{
    /*Puts the part after ur cursor (until relLen is 0, but u prob used a cursor dependent function for it i hope) in a calloc*/
    int where = ftell(file);

    void *fileOff = calloc(sizeof(char), relLen + 1);
    fread(fileOff, relLen, 1, file);

    fseek(file, where, SEEK_SET);
    return fileOff;
}

void *_putUrFileOffsetAfterReplacedData(FILE *file, int relLen, int replaceLen)
{
    /*Let's say u want to insert another file in a file, ur cursor is placed to where u want and either u defined the length of the data u want to replace
    this func gives u a pointer containing all the not replaced data u want after the write area (so the rest of the file after the replaced data if relLen is gotten by the intended method)*/

    relLen = relLen - replaceLen;
    fseek(file, replaceLen, SEEK_CUR);
    void *fileOff = _putUrFileOffsetInAFXXXingPointer(file, relLen);

    fseek(file, (replaceLen * (-1)), SEEK_CUR);
    return fileOff;
}
void *putFileInPointer(FILE *file)
{ // you can read
    int len = getFileLength(file);
    void *pFile = calloc(sizeof(char), len + 1);
    fread(pFile, len, 1, file);
    return pFile;
}
void *getRestOfFileWithOffsetAlsoSetCursorToAddrOfReplacedData(FILE *file, int addrOfStartOfReplData, int numOfDataReplaced)
{
    // fseek(file, addrOfStartOfReplData, SEEK_SET); // put this for the getRelativeFileLength func
    void *fileOffNotReplaced = _putUrFileOffsetAfterReplacedData(file, _getRelativeFileLength(file, addrOfStartOfReplData), numOfDataReplaced);

    return fileOffNotReplaced;
}
