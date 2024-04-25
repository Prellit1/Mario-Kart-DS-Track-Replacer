
#include <windows.h>
#include <CommCtrl.h>
#include "common.h"
#include "carcPut.h"
#include "putFile.h"
#include "ARM9/ARM9.h"
#include "MusicREPL/soundarray.h"
#include "LZ/compCarcBmg.h"
#include <shlobj.h>

#pragma once

#define TRACKS 43
#define TRACKSMUS 29
#define CARC 1
#define MKDS 0
#define TEMP_NAME "C:\\Users\\UserPC\\OneDrive\\Bureau\\GET DDATA\\Track Replacer\\temporary.carc"
// #define CBSING 0x10
extern HWND hWnd;
extern HWND hwndCBS, langCBS, musicCBS;
extern OPENFILENAME openerM, openerC, openerNCLR, openerNCGR, openerNSCR;
extern FILE *MkdsF, *CarcF, *ncgr, *nclr, *nscr;
extern int isTex;
extern int isFold;

int trackReplacing();
int dialogForCarc(int CarcOrMkds);
int dialogForNC(int NC);
int NCReplacing(int curs);
void decompArm9ToRom(FILE *Mkds, HWND Hwnd);
int checkDecomp(FILE *Mkds);
extern char TrckNames[][50];
int LocGlobRepl(int BX, int BY, int TX, int TY, int isLocal);
int nameReplacing(char *Thingy, unsigned int length);
extern unsigned char TrckId[];
extern char TrckMusNames[][30];
int musRepl(int trackCur, int musCur, int bankID);
int folder_track_replacing(char *folder);
