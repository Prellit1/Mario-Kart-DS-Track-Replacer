#include "common.h"

#pragma once
#define FAT_LEN 610
enum
{
    AIRSHIP = 35,
    AWARD = 37,
    YOSHFALL = 39,

    BEACHnDL = 41,
    BEACHDL = 42,

    TICKTOCK = 45,

    CROSSnDL = 47,
    CROSSDL = 48,

    DESERT = 51,

    UNUDOKAN = 53,
    UNUWAHB = 55,

    GARDEN = 57,
    KOOPA = 59,

    UNUCHEESE = 61,

    MANSIONnDL = 63,
    MANSIONDL = 64,
    MARIOCIRCUIT = 67,

    MINIBLOCK64 = 69,
    MINIBLOCKCOURSE = 71,
    MINIDOKANGC = 73,

    MINISTAGE1 = 75,
    MINISTAGE2 = 77,
    MINISTAGE3 = 79,
    MINISTAGE4 = 81,
    MRSTAGE1 = 83,
    MRSTAGE2 = 85,
    MRSTAGE3 = 87,
    MRSTAGE4 = 89,

    UNUNOKO = 91,

    OLDBABY = 93,
    OLDCHOCO64 = 95,
    OLDCHOCOSFC = 97,
    OLDDONUT = 97 + 2,
    OLDFRAPPE = 99 + 2,
    OLDHYUDORO = 101 + 2,
    OLDKINOKO = 103 + 2,
    OLDKOOPAAGB = 105 + 2,
    OLDLUIGIAGB = 107 + 2,

    OLDLUIGIGCnDL = 109 + 2,
    OLDLUIGIGCDL = 110 + 2,

    UNUOMARIOGC = 113 + 2,

    OLDMARIOSFC = 115 + 2,

    OLDMOMO64nDL = 117 + 2,
    OLDMOMO64DL = 118 + 2,

    OLDNOKOSFC = 121 + 2,
    OLDPEACHAGB = 123 + 2,
    OLDSKYAGB = 125 + 2,
    OLDYOSHIGC = 127 + 2,

    PINBALL = 129 + 2,
    RAINBOW = 131 + 2,
    RIDGE = 133 + 2,
    DKPASS = 135 + 2,
    STADIUM = 137 + 2,

    STAFFROLL = 139 + 2,
    STAFFROLLTR = 141 + 2,

    UNUTESTCIRCLE = 143 + 2,
    UNUTESTRAINBOW = 145 + 2,

    TOWN = 147 + 2,

    UNUWARIOS = 151 + 2,

} // RACE_ID;
;
extern char trackIdConv[];
char getGameCourseID(int ID);

typedef struct
{
    unsigned int addressStart;
    unsigned int addressEnd;
} FATID;

typedef struct
{
    unsigned int size;
    unsigned int addressStart;
    unsigned int id;

} ENTRY_FAT;

void getFat(FILE *mkds);
ENTRY_FAT getCourse(int id);
ENTRY_FAT getCourseTex(int idCourse);
void moveAddrRelatToStartAddr(ENTRY_FAT course, unsigned int newSize, FILE *file);
extern FATID *allTheFat;
extern int fatSize;
void freeFat();