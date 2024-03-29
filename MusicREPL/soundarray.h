#include "../ARM9/ARM9.h"
#include "../common.h"
/*

START OF COPIED CODE

*/

// Vanilla Sequence IDs:
#define NO_SEQ 255
#define SEQ_TITLE 0
#define SEQ_SELECT 1
#define SEQ_MULTIPLAYER 2
#define SEQ_WIFI 3
#define SEQ_OPTIONS 4
#define SEQ_5 5
#define SEQ_RECORDS 6
#define SEQ_START_FANFARE 7
#define SEQ_COURSE_IN_FANFARE 8
#define SEQ_COURSE_IN_FANFARE2 9
#define SEQ_COURSE_IN_FANFARE3 10
#define SEQ_START_FANFARE_TA 11
#define SEQ_START_FANFARE_MG_MR 12
#define SEQ_COURSE_IN_FANFARE_MG 13
#define SEQ_COURSE_IN_FANFARE_BOSS 14
#define SEQ_BEACH 15
#define SEQ_JUNGLE 16
#define SEQ_TOWN 17
#define SEQ_AIRSHIP 18
#define SEQ_STADIUM 19
#define SEQ_GARDEN 20
#define SEQ_OBAKE 21
#define SEQ_CIRCUIT_SFC 22
#define SEQ_BEACH_SFC 23
#define SEQ_NOHARA_SFC 24
#define SEQ_DART_SFC 25
#define SEQ_CIRCUIT_AGB 26
#define SEQ_KOOPA_AGB 27
#define SEQ_SKY_AGB 28
#define SEQ_MOUNTAIN 29
#define SEQ_COUNTRY 30
#define SEQ_SNOW_64 31
#define SEQ_OBAKE_64 32
#define SEQ_CIRCUIT_GC 33
#define SEQ_BABYPARK 34
#define SEQ_BRIDGE 35
#define SEQ_RIDGEWAY 36
#define SEQ_SNOW 37
#define SEQ_DESERT 38
#define SEQ_CLOCK 39
#define SEQ_KOOPA 40
#define SEQ_RAINBOW 41
#define SEQ_WANWAN 42
#define SEQ_MINIGAME 43
#define SEQ_MISSION_RUN 44
#define SEQ_MISSION_RUN_BOSS 45
#define SEQ_1STGOAL 46
#define SEQ_2NDGOAL 47
#define SEQ_DAMEGOAL 48
#define SEQ_DAMEGOAL2 49
#define SEQ_1STGOAL2 50
#define SEQ_3RDGOAL 51
#define SEQ_MR_GOAL 52
#define SEQ_MR_GOAL2 53
#define SEQ_MR_GOAL3 54
#define SEQ_BOSS_GOAL 55
#define SEQ_GOAL_RESULT1 56
#define SEQ_GOAL_RESULT2 57
#define SEQ_GOAL_RESULT3 58
#define SEQ_MR_RESULT1 59
#define SEQ_GOAL_RESULT4 60
#define SEQ_GOAL_RESULT5 61
#define SEQ_FINALLAP 62
#define SEQ_RESULT1 63
#define SEQ_RESULT2 64
#define SEQ_RESULT3 65
#define SEQ_AWARD 66
#define SEQ_TROPHY 67
#define SEQ_TROPHY2 68
#define SEQ_STAFFROLL 69
#define SEQ_STAFFROLL_TRUE 70
#define SEQ_SECRET1 71
#define SEQ_SECRET2 72
#define SEQ_STAR 73
#define SEQ_CIRCUIT 74
#define SEQ_LOGO 75

// Vanilla SFX Bank IDs:
#define NO_BANK 255
#define BANK_SE_CROSS 16
#define BANK_SE_BANK 17
#define BANK_SE_BEACH 18
#define BANK_SE_MANSION 19
#define BANK_SE_DESERT 20
#define BANK_SE_TOWN 21
#define BANK_SE_PINBALL 22
#define BANK_SE_BRIDGE 23
#define BANK_SE_SNOW 24
#define BANK_SE_CLOCK 25
#define BANK_SE_MARIO 26
#define BANK_SE_AIRSHIP 27
#define BANK_SE_STADIUM 28
#define BANK_SE_GARDEN 29
#define BANK_SE_KOOPA 30
#define BANK_SE_FARM 31
#define BANK_SE_HYUDORO 32
#define BANK_SE_LUIGI_AGB 33
#define BANK_SE_EDIT 35
#define BANK_SE_WIFI 36
#define BANK_SE_MINIGAME 43
#define BANK_SE_RAINBOW 48

// Unknown1
#define UNKNOWN1_TRUE 2
#define UNKNOWN1_FALSE 255
// Unknown2
#define UNKNOWN2_TRUE 34
#define UNKNOWN2_FALSE 255

typedef struct
{
    unsigned char unk1;
    unsigned char sseqID;
    unsigned char bankID;
    unsigned char unk2;
} Mus_tabl_entr;

/*

END OF COPIED CODE

*/
extern unsigned char cursorToTableID[];
void replMusic(FILE *mkds, int cursTrackRepl, int cursTrackMus, int bankID);