#include "windowST.h"

OPENFILENAME openerM, openerC, openerNCLR, openerNCGR, openerNSCR;
FILE *MkdsF, *CarcF, *ncgr, *nclr, *nscr;
HWND hWnd;
HWND hwndCBS, langCBS, musicCBS;
int isTex;

char TrckMusNames[TRACKSMUS][30] = {

    "Circuit",
    "Yoshi Falls",
    "Beach",
    "Mansion",

    "Desert",
    "Delfino Squares",
    "Stadium/Pinball",
    "Shroom Ridge",

    "DK Pass",
    "Tick Tock Clock",
    "Airship Fortress",

    "Peach Garden",
    "Bowser Castle",
    "Rainbow Road",

    "SNES Mario Circuit",
    "N64 Moo Moo Farm",
    "GBA Circuit",
    "GCN Luigi Circuit",

    "SNES Donut Plains",
    "N64 Frappe Snowland",
    "GBA Bowser Castle",
    "GCN Baby Park",

    "SNES Koopa Beach",
    "N64 Choco Mountain",
    "GCN Mushroom Bridge",

    "SNES Choco Island",
    "N64 Banshee Broadwalk",
    "GBA Sky Garden",

    "Battle",

};
char TrckNames[TRACKS][50] = {

    "Figure 8 Circuit",
    "Figure 8 Circuit (Download Play)",
    "Yoshi Falls",
    "Cheep Cheep Beach",
    "Cheep Cheep Beach (Download Play)",
    "Luigi Mansion",
    "Luigi Mansion (Download Play)",

    "Desert Hills",
    "Delfino Square",
    "Waluigi Pinball",
    "Shroom Ridge",

    "DK Pass",
    "Tick Tock Clock",
    "Mario Circuit",
    "Airship Fortress",

    "Wario Stadium",
    "Peach Garden",
    "Bowser Castle",
    "Rainbow Road",

    "SNES Mario Circuit 1",
    "N64 Moo Moo Farm",
    "N64 Moo Moo Farm (Download Play)",
    "GBA Peach Circuit",
    "GCN Luigi Circuit",
    "GCN Luigi Circuit (Download Play)",

    "SNES Donut Plains 1",
    "N64 Frappe Snowland",
    "GBA Bowser Castle 2",
    "GCN Baby Park",

    "SNES Koopa Beach 2",
    "N64 Choco Mountain",
    "GBA Luigi Circuit",
    "GCN Mushroom Bridge",

    "SNES Choco Island 2",
    "N64 Banshee Broadwalk",
    "GBA Sky Garden",
    "GCN Yoshi Circuit",

    "Block Fort",
    "Pipe Plaza",
    "Nintendo DS",
    "Twilight House",
    "Tart Top",
    "Palm Shore",
};

unsigned char TrckId[TRACKS] = {
    CROSSnDL,
    CROSSDL,
    YOSHFALL,
    BEACHnDL,
    BEACHDL,
    MANSIONnDL,
    MANSIONDL,

    DESERT,
    TOWN,
    PINBALL,
    RIDGE,

    DKPASS,
    TICKTOCK,
    MARIOCIRCUIT,
    AIRSHIP,

    STADIUM,
    GARDEN,
    KOOPA,
    RAINBOW,

    OLDMARIOSFC,
    OLDMOMO64nDL,
    OLDMOMO64DL,
    OLDPEACHAGB,
    OLDLUIGIGCnDL,
    OLDLUIGIGCDL,

    OLDDONUT,
    OLDFRAPPE,
    OLDKOOPAAGB,
    OLDBABY,

    OLDNOKOSFC,
    OLDCHOCO64,
    OLDLUIGIAGB,
    OLDKINOKO,

    OLDCHOCOSFC,
    OLDHYUDORO,
    OLDSKYAGB,
    OLDYOSHIGC,

    MINIBLOCK64,
    MINIDOKANGC,
    MINISTAGE1,
    MINISTAGE2,
    MINISTAGE3,
    MINISTAGE4,

};
int trackReplacing()
{
    if (CarcF && MkdsF)
    {
        int a = SendMessage(hwndCBS, CB_GETCURSEL, 0, 0);
        if (a == 8)
        {
            int resp = MessageBox(hWnd, "Warning : If the carc that contains the model and the map data doesn't have a bridge object, the game will crash on this slot. Proceed ?", "Warning", MB_ICONWARNING | MB_YESNO);
            if (resp == IDNO)
                return 2;
        }
        /* printf("%u, %u %d    %d    %d\n", a, PINBALL, PINBALL, TrckId[a], TrckId[9]); */
        putCarcInRom(CarcF, MkdsF, TrckId[a], isTex);
        openerC.lpstrFile = "";
        // openerM.lpstrFile = "";
        // SetWindowTextA(texTextC, openerC.lpstrFile);
        // SetWindowTextA(texTextM, openerM.lpstrFile);
        //  fclose(MkdsF);
        fclose(CarcF);
        return 1;
    }
    return 0;
}

int modifCurs(int curs)
{
    if (curs > 23)
        curs--;
    if (curs > 20)
        curs--;
    if (curs > 5)
        curs--;
    if (curs > 3)
        curs--;
    if (curs > 0)
        curs--;
    return curs;
}
int NCReplacing(int curs)
{
    curs = modifCurs(curs);
    getFat(MkdsF);
    if (nscr && nclr && ncgr)
    {

        /* printf("%d,   %d,   %d\n", Ncgr.addressStart, Nclr.addressStart, Nscr.addressStart);
printf("%d,   %d,   %d\n", Ncgr.size, Nclr.size, Nscr.size);
printf("%d,   %d,   %d\n", Ncgr.id, Nclr.id, Nscr.id); */
        ENTRY_FAT Ncgr = getCourse(153 + curs * 3);
        putFileInRom(ncgr, MkdsF, Ncgr);
        getFat(MkdsF);

        ENTRY_FAT Nclr = getCourse(153 + 1 + curs * 3);
        putFileInRom(nclr, MkdsF, Nclr);
        getFat(MkdsF);

        ENTRY_FAT Nscr = getCourse(153 + 2 + curs * 3);
        putFileInRom(nscr, MkdsF, Nscr);
        getFat(MkdsF);
        openerNCGR.lpstrFile = "";
        openerNCLR.lpstrFile = "";
        openerNSCR.lpstrFile = "";
        // openerM.lpstrFile = "";
        // SetWindowTextA(texTextC, openerC.lpstrFile);
        // SetWindowTextA(texTextM, openerM.lpstrFile);
        //  fclose(MkdsF);
        fclose(ncgr);
        fclose(nclr);
        fclose(nscr);
        freeFat();
        return 1;
    }
    return 0;
}
int dialogForCarc(int CarcOrMkds)
{
    OPENFILENAME *opener;
    // common dialog box structure
    if (CarcOrMkds == MKDS)
    {
        opener = &openerM;
    }
    else
    {
        opener = &openerC;
    }

    TCHAR szFile[360] = {0};

    ZeroMemory(opener, sizeof(*opener));
    opener->lStructSize = sizeof(*opener);
    opener->hwndOwner = hWnd;
    opener->lpstrFile = szFile;
    opener->nMaxFile = sizeof(szFile);

    if (CarcOrMkds == MKDS)
    {
        opener->lpstrFilter = "NDS ROM\0*.nds\0";
    }
    else
    {
        opener->lpstrFilter = "CARC\0*.carc\0";
    }

    opener->nFilterIndex = 1;
    opener->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(opener) == TRUE)
    {
        // use ofn.lpstrFile
        if (CarcOrMkds == MKDS)
        {
            if (MkdsF)
                fclose(MkdsF);
            MkdsF = fopen(opener->lpstrFile, "rb+");
        }
        else
        {
            if (CarcF)
                fclose(CarcF);
            CarcF = fopen(opener->lpstrFile, "rb+");
        }
        return 1;
    }
    if (!CommDlgExtendedError())
        return 2;
    return 0;
}

int dialogForNC(int NC)
{
    OPENFILENAME *opener;
    // common dialog box structure
    switch (NC)
    {
    case 0:
        opener = &openerNCGR;
        break;
    case 1:
        opener = &openerNCLR;
        break;

    default:
        opener = &openerNSCR;
        break;
    }

    TCHAR szFile[360] = {0};

    ZeroMemory(opener, sizeof(*opener));
    opener->lStructSize = sizeof(*opener);
    opener->hwndOwner = hWnd;
    opener->lpstrFile = szFile;
    opener->nMaxFile = sizeof(szFile);

    switch (NC)
    {
    case 0:
        opener->lpstrFilter = "NCGR\0*.NCGR\0";
        break;
    case 1:
        opener->lpstrFilter = "NCLR\0*.NCLR\0";
        break;

    default:
        opener->lpstrFilter = "NSCR\0*.NSCR\0";
        break;
    }

    opener->nFilterIndex = 1;
    opener->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(opener) == TRUE)
    {
        // use ofn.lpstrFile
        switch (NC)
        {
        case 0:
            if (ncgr)
                fclose(ncgr);
            ncgr = fopen(opener->lpstrFile, "rb+");
            break;
        case 1:
            if (nclr)
                fclose(nclr);
            nclr = fopen(opener->lpstrFile, "rb+");
            break;

        default:
            if (nscr)
                fclose(nscr);
            nscr = fopen(opener->lpstrFile, "rb+");
            break;
        }
        return 1;
    }
    if (!CommDlgExtendedError())
        return 2;
    return 0;
}

void decompArm9ToRom(FILE *Mkds, HWND Hwnd)
{
    getFat(MkdsF);
    int addr = getArm9Addr(Mkds);
    int size = getArm9Size(Mkds);
    NFooter NF = getArm9Footer(Mkds);
    char *arm9 = getArm9InAlloc(Mkds);
    char *decompArm9 = COPIED_Decompress(arm9, NF.OffsetToArm9Addr, size);
    ENTRY_FAT fakeFat;
    fakeFat.addressStart = addr;
    fakeFat.size = size;
    fakeFat.id = 0xff;
    putDataInRom(leng, decompArm9, Mkds, fakeFat);
    freeFat();
    free(arm9);
}
int checkDecomp(FILE *Mkds)
{
    int a;
    fseek(Mkds, 0x4b60, SEEK_SET); // check the decomp footer or smth
    fread(&a, sizeof(int), 1, Mkds);

    if (!a)
        return 1;
    return 0;
}

int LocGlobRepl(int BX, int BY, int TX, int TY, int isLocal)
{
    if (MkdsF)
    {

        int ID = TrckId[SendMessage(hwndCBS, CB_GETCURSEL, 0, 0)];
        ID = getGameCourseID(ID) + 1;
        editGlobalLocalCoord(isLocal, (unsigned short)BX, (unsigned short)BY, (unsigned short)TX, (unsigned short)TY, MkdsF, ID);
        return 1;
    }

    return 0;
}
int nameReplacing(char *Thingy, unsigned int length)
{
    if (MkdsF)
    {
        int trackId = SendMessage(hwndCBS, CB_GETCURSEL, 0, 0);
        if (trackId >= 37)
        {
            MessageBox(hWnd, "Warning : Only the name during the intro will change as Battle Tracks' names on the menu are graphics and not really text", "Warning", MB_ICONWARNING);
        }
        trackId = modifCurs(trackId);
        int langID = SendMessage(langCBS, CB_GETCURSEL, 0, 0);
        MKDSReplTrack(MkdsF, langID, trackId, length, Thingy);

        return 1;
    }
    return 0;
}

int musRepl(int trackCur, int musCur, int bankID)
{
    trackCur = modifCurs(trackCur);
    if (cursorToTableID[trackCur] == 39)
    {
        return 2;
    }
    if (bankID && (bankID < 16 || bankID > 33) && bankID != 43 && bankID != 48)
    {
        return 0;
    }
    replMusic(MkdsF, trackCur, musCur, bankID);
    return 1;
}