#include "windowST.h"

OPENFILENAME openerM, openerC, openerNCLR, openerNCGR, openerNSCR;
FILE *MkdsF, *CarcF, *ncgr, *nclr, *nscr;
HWND hWnd;
HWND hwndCBS;
int isTex;
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

char TrckId[TRACKS] = {
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
    if (openerC.lpstrFile[0] != '\0' && openerM.lpstrFile[0] != '\0')
    {
        int a = SendMessage(hwndCBS, CB_GETCURSEL, 0, 0);
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
int NCReplacing(int curs)
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

    if (nscr && nclr && ncgr && MkdsF)
    {
        getFat(MkdsF);
        ENTRY_FAT Ncgr = getCourse(153 + curs * 3);
        ENTRY_FAT Nclr = getCourse(153 + 1 + curs * 3);
        ENTRY_FAT Nscr = getCourse(153 + 2 + curs * 3);

        // printf("%d,   %d,   %d\n", Ncgr.addressStart, Nclr.addressStart, Nscr.addressStart);
        putFileInRom(ncgr, MkdsF, Ncgr);
        putFileInRom(nclr, MkdsF, Nclr);
        putFileInRom(nscr, MkdsF, Nscr);
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