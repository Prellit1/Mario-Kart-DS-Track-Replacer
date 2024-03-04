#include "../common.h"
typedef struct
{
    unsigned int addrStart;
    unsigned int addrEnd;
} ENTRY;
typedef struct
{
    unsigned int numFile;
    unsigned int *addresses;
} ADDRFAT;
static inline unsigned int getSize(ENTRY ent)
{
    return ent.addrEnd - ent.addrStart;
}

static inline unsigned short getNumBmg(char *bmg)
{
    return *(unsigned short *)&bmg[0x20 + 0x8];
}
