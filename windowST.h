
#include <windows.h>
#include <CommCtrl.h>
#include "common.h"
#include "carcPut.h"
#include "putFile.h"
#include "ARM9/ARM9.h"
#pragma once

#define TRACKS 43
#define CARC 1
#define MKDS 0
// #define CBSING 0x10
extern HWND hWnd;
extern HWND hwndCBS;
extern OPENFILENAME openerM, openerC, openerNCLR, openerNCGR, openerNSCR;
extern FILE *MkdsF, *CarcF, *ncgr, *nclr, *nscr;
extern int isTex;

int trackReplacing();
int dialogForCarc(int CarcOrMkds);
int dialogForNC(int NC);
int NCReplacing(int curs);
void decompArm9ToRom(FILE *Mkds, HWND Hwnd);
int checkDecomp(FILE *Mkds);
extern char TrckNames[TRACKS][50];
int LocGlobRepl(int BX, int BY, int TX, int TY, int isLocal);

extern char TrckId[TRACKS];