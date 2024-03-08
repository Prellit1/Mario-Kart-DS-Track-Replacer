#include "soundarray.h"

/* ncp_over(0x0215316C) */

unsigned char cursorToMusicID[] = {
    SEQ_CIRCUIT,
    SEQ_JUNGLE,
    SEQ_BEACH,
    SEQ_OBAKE,

    SEQ_DESERT,
    SEQ_TOWN,
    SEQ_STADIUM,
    SEQ_RIDGEWAY,

    SEQ_SNOW,
    SEQ_CLOCK,

    SEQ_AIRSHIP,

    SEQ_GARDEN,
    SEQ_KOOPA,
    SEQ_RAINBOW,

    SEQ_CIRCUIT_SFC,
    SEQ_COUNTRY,
    SEQ_CIRCUIT_AGB,
    SEQ_CIRCUIT_GC,

    SEQ_NOHARA_SFC,
    SEQ_SNOW_64,
    SEQ_KOOPA_AGB,
    SEQ_BABYPARK,

    SEQ_BEACH_SFC,
    SEQ_MOUNTAIN,
    SEQ_BRIDGE,

    SEQ_DART_SFC,
    SEQ_OBAKE_64,
    SEQ_SKY_AGB,

    SEQ_MINIGAME,
}; // Have to do the Combobox
unsigned char cursorToTableID[] = {
    7,
    11,
    10,
    20,

    31,
    15,
    32,
    27,

    30,
    33,
    34,
    16,

    17,
    19,
    37,
    35,

    21,
    13,
    25,
    8,

    23,
    14,
    36,
    12,

    22,
    28,
    26,
    18,

    24,
    29,
    38,
    9,

    39,
    39,
    39,
    39,
    39,
    39,
};
/* Mus_tabl_entr SoundArray[] =
    {
        {UNKNOWN1_TRUE, NO_SEQ, NO_BANK, UNKNOWN2_FALSE},                     // Invalid
        {UNKNOWN1_TRUE, SEQ_COURSE_IN_FANFARE2, NO_BANK, UNKNOWN2_FALSE},     // Special Course Intro
        {UNKNOWN1_TRUE, SEQ_COURSE_IN_FANFARE, NO_BANK, UNKNOWN2_FALSE},      // Normal course Intro
        {UNKNOWN1_TRUE, SEQ_COURSE_IN_FANFARE3, NO_BANK, UNKNOWN2_FALSE},     // Stadium course Intro
        {UNKNOWN1_TRUE, SEQ_COURSE_IN_FANFARE, NO_BANK, UNKNOWN2_FALSE},      // Special Course Intro Duplicate (Meant for a scrapped Waluigi Pinball course intro SEQ)
        {UNKNOWN1_TRUE, SEQ_COURSE_IN_FANFARE_MG, NO_BANK, UNKNOWN2_FALSE},   // Battle Stage Course Intro
        {UNKNOWN1_TRUE, SEQ_COURSE_IN_FANFARE_BOSS, NO_BANK, UNKNOWN2_FALSE}, // Mission Boss Intro
        {UNKNOWN1_FALSE, SEQ_CIRCUIT, BANK_SE_CROSS, UNKNOWN2_FALSE},         // cross_course
        {UNKNOWN1_FALSE, SEQ_CIRCUIT_GC, BANK_SE_GARDEN, UNKNOWN2_FALSE},     // old_luigi_gc
        {UNKNOWN1_FALSE, SEQ_CIRCUIT_GC, BANK_SE_MARIO, UNKNOWN2_FALSE},      // old_yoshi_gc
        {UNKNOWN1_FALSE, SEQ_BEACH, BANK_SE_BEACH, UNKNOWN2_FALSE},           // beach_course
        {UNKNOWN1_FALSE, SEQ_JUNGLE, BANK_SE_BANK, UNKNOWN2_FALSE},           // bank_course
        {UNKNOWN1_FALSE, SEQ_BABYPARK, NO_BANK, UNKNOWN2_FALSE},              // old_baby_gc
        {UNKNOWN1_FALSE, SEQ_COUNTRY, BANK_SE_FARM, UNKNOWN2_FALSE},          // old_momo_64
        {UNKNOWN1_FALSE, SEQ_SNOW_64, BANK_SE_SNOW, UNKNOWN2_FALSE},          // old_frappe_64
        {UNKNOWN1_FALSE, SEQ_TOWN, BANK_SE_TOWN, UNKNOWN2_FALSE},             // town_course
        {UNKNOWN1_FALSE, SEQ_AIRSHIP, BANK_SE_AIRSHIP, UNKNOWN2_FALSE},       // airship_course
        {UNKNOWN1_FALSE, SEQ_STADIUM, BANK_SE_STADIUM, UNKNOWN2_FALSE},       // stadium_course
        {UNKNOWN1_FALSE, SEQ_BRIDGE, BANK_SE_BRIDGE, UNKNOWN2_FALSE},         // old_kinoko_gc
        {UNKNOWN1_FALSE, SEQ_GARDEN, BANK_SE_GARDEN, UNKNOWN2_FALSE},         // garden_course
        {UNKNOWN1_FALSE, SEQ_OBAKE, BANK_SE_MANSION, UNKNOWN2_FALSE},         // mansion_course
        {UNKNOWN1_FALSE, SEQ_CIRCUIT_SFC, BANK_SE_MARIO, UNKNOWN2_FALSE},     // old_mario_sfc
        {UNKNOWN1_FALSE, SEQ_BEACH_SFC, BANK_SE_BEACH, UNKNOWN2_FALSE},       // old_noko_sfc
        {UNKNOWN1_FALSE, SEQ_NOHARA_SFC, BANK_SE_MARIO, UNKNOWN2_FALSE},      // old_donut_sfc
        {UNKNOWN1_FALSE, SEQ_DART_SFC, BANK_SE_MARIO, UNKNOWN2_FALSE},        // old_choco_sfc
        {UNKNOWN1_FALSE, SEQ_CIRCUIT_AGB, NO_BANK, UNKNOWN2_FALSE},           // old_peach_agb
        {UNKNOWN1_FALSE, SEQ_CIRCUIT_AGB, BANK_SE_LUIGI_AGB, UNKNOWN2_FALSE}, // old_luigi_agb
        {UNKNOWN1_FALSE, SEQ_RIDGEWAY, BANK_SE_BRIDGE, UNKNOWN2_FALSE},       // ridge_course
        {UNKNOWN1_FALSE, SEQ_MOUNTAIN, BANK_SE_SNOW, UNKNOWN2_FALSE},         // old_choco_64
        {UNKNOWN1_FALSE, SEQ_OBAKE_64, BANK_SE_HYUDORO, UNKNOWN2_FALSE},      // old_hyudoro_64
        {UNKNOWN1_FALSE, SEQ_SNOW, BANK_SE_SNOW, UNKNOWN2_FALSE},             // snow_course
        {UNKNOWN1_FALSE, SEQ_DESERT, BANK_SE_DESERT, UNKNOWN2_FALSE},         // desert_course
        {UNKNOWN1_FALSE, SEQ_STADIUM, BANK_SE_PINBALL, UNKNOWN2_FALSE},       // pinball_course
        {UNKNOWN1_FALSE, SEQ_CLOCK, BANK_SE_CLOCK, UNKNOWN2_FALSE},           // clock_course
        {UNKNOWN1_FALSE, SEQ_CIRCUIT, BANK_SE_MARIO, UNKNOWN2_FALSE},         // mario_course
        {UNKNOWN1_FALSE, SEQ_RAINBOW, BANK_SE_RAINBOW, UNKNOWN2_FALSE},       // rainbow_course
        {UNKNOWN1_FALSE, SEQ_KOOPA_AGB, NO_BANK, UNKNOWN2_FALSE},             // old_koopa_agb
        {UNKNOWN1_FALSE, SEQ_KOOPA, BANK_SE_KOOPA, UNKNOWN2_FALSE},           // koopa_course
        {UNKNOWN1_FALSE, SEQ_SKY_AGB, NO_BANK, UNKNOWN2_FALSE},               // old_sky_agb
        {UNKNOWN1_FALSE, SEQ_MINIGAME, BANK_SE_MINIGAME, UNKNOWN2_TRUE},      // all mini stages
        {UNKNOWN1_FALSE, SEQ_MISSION_RUN_BOSS, NO_BANK, UNKNOWN2_TRUE},       // all mr stages
        {UNKNOWN1_TRUE, SEQ_RESULT1, NO_BANK, UNKNOWN2_FALSE},                // Trophy Screen 1st Place
        {UNKNOWN1_TRUE, SEQ_AWARD, NO_BANK, UNKNOWN2_FALSE},                  // Award
        {UNKNOWN1_TRUE, SEQ_STAFFROLL, NO_BANK, UNKNOWN2_FALSE},              // StaffRoll
        {UNKNOWN1_TRUE, SEQ_STAFFROLL_TRUE, NO_BANK, UNKNOWN2_FALSE},         // StaffRollTrue
        {UNKNOWN1_TRUE, SEQ_WIFI, BANK_SE_WIFI, UNKNOWN2_FALSE},              // Nintendo Wifi Match Menu
        {UNKNOWN1_TRUE, SEQ_MULTIPLAYER, NO_BANK, UNKNOWN2_FALSE},            // Local Multiplayer Setup menu
        {UNKNOWN1_TRUE, SEQ_RECORDS, NO_BANK, UNKNOWN2_FALSE},                // Records menu
        {UNKNOWN1_TRUE, SEQ_OPTIONS, BANK_SE_EDIT, UNKNOWN2_FALSE},           // Options Menu & Emblem Editor
        {UNKNOWN1_TRUE, SEQ_LOGO, NO_BANK, UNKNOWN2_FALSE},                   // Nintendo Logo screen
        {UNKNOWN1_TRUE, SEQ_SELECT, NO_BANK, UNKNOWN2_FALSE},                 // Main Menu
        {UNKNOWN1_TRUE, NO_SEQ, NO_BANK, UNKNOWN2_FALSE},                     // Unknown
        {UNKNOWN1_FALSE, SEQ_CIRCUIT, NO_BANK, UNKNOWN2_FALSE}                // Unknown
}; */

Mus_tabl_entr getAppropTableEntry(int cursTrackRepl, int cursTrackMus, int bankID, FILE *mkds)
{
    Mus_tabl_entr Result;
    if (!bankID)
    {
        int temp = ftell(mkds);
        int addr = getArm9Addr(mkds);

        fseek(mkds, addr + 0x15316C, SEEK_SET);
        fseek(mkds, sizeof(Mus_tabl_entr) * cursorToTableID[cursTrackRepl], SEEK_CUR);
        fread(&Result, sizeof(Mus_tabl_entr), 1, mkds);

        fseek(mkds, temp, SEEK_SET);
        Result.sseqID = cursorToMusicID[cursTrackMus];

        return Result;
    }

    Result.unk1 = UNKNOWN1_FALSE;
    Result.unk2 = UNKNOWN2_FALSE;
    Result.sseqID = cursorToMusicID[cursTrackMus];
    Result.bankID = bankID;
    return Result;
}

void replMusic(FILE *mkds, int cursTrackRepl, int cursTrackMus, int bankID)
{
    int addr = getArm9Addr(mkds);
    fseek(mkds, addr + 0x15316C, SEEK_SET);
    Mus_tabl_entr replacer = getAppropTableEntry(cursTrackRepl, cursTrackMus, bankID, mkds);
    fseek(mkds, sizeof(Mus_tabl_entr) * cursorToTableID[cursTrackRepl], SEEK_CUR);
    fwrite(&replacer, sizeof(Mus_tabl_entr), 1, mkds);
}