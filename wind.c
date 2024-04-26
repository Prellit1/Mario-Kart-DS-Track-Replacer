
#include "FileAdd.h"
#include "putFile.h"
#include "carcPut.h"
#include "windowST.h"
#include "res.h"

HWND texText, texTextM, texTextC, texTextN, texTextN2, texTextN3, mapText, hBX, hBY, hTX, hTY, hName, bank;
char isLocal;
int isFold;
#define CARC_IMPORT 0x2
#define MKDS_IMPORT 0x4
#define NCGR 0x40
#define NCLR 0x20
#define NSCR 0x80

#define TEX_BOX 0x10
#define MAP_PUSH 0x200
#define MAP_REP 0x400
#define FILE_REP 0x8
#define NC_REP 0x100
#define RENAME 0x800
#define MUS_REP 0x1000
#define FOLD_BOX 0x2000

#define OFFSET 100 // Kinda hacky or smth but hey it modifies faster the x pos of the map stufff
// FUCK IT WE BALL
#define CARC_Y 245
#define BASE_X 30
#define NC_Y 365
#define TXT_YOFFS 6
#define TRACKNNAME_Y 80
#define MKDS_Y 20
#define MAP_Y 550

LRESULT CALLBACK WindProce(HWND, UINT, WPARAM, LPARAM);
BOOL EnforceSignedIntegerEdit(HWND);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    openerC.lpstrFile = "";
    openerM.lpstrFile = "";
    WNDCLASSW wc = {0};
    HBRUSH yello = CreateSolidBrush(RGB(0x0c, 0x0c, 0x0c));
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"windo";
    wc.lpfnWndProc = WindProce;
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IC_ICON));

    if (!(RegisterClassW(&wc)))
        return -1;

    hWnd = CreateWindowW(L"windo", L"Track Replacer 1.0", WS_OVERLAPPEDWINDOW | WS_VISIBLE, BASE_X + 50, 100, 1000, 700, NULL, NULL, NULL, NULL);

    texTextM = CreateWindowW(L"STATIC", L"MKDS", WS_VISIBLE | WS_CHILD, BASE_X + 125, MKDS_Y + TXT_YOFFS, 1200, 27, hWnd, NULL, NULL, NULL);
    HWND hwndButtonMkds = CreateWindowW(L"BUTTON", L"Import MKDS", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, MKDS_Y, 100, 30, hWnd, (HMENU)MKDS_IMPORT, NULL, NULL);

    HWND hwndButtonTex = CreateWindowW(L"BUTTON", L"Is Tex Carc ? :", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CHECKBOX, BASE_X, CARC_Y - 30, 120, 30, hWnd, (HMENU)TEX_BOX, NULL, NULL);
    HWND hwndButtonFOLD = CreateWindowW(L"BUTTON", L"Folder import :", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CHECKBOX, BASE_X, CARC_Y - 60, 150, 30, hWnd, (HMENU)FOLD_BOX, NULL, NULL);
    texText = CreateWindowW(L"STATIC", L"Not Tex Carc", WS_VISIBLE | WS_CHILD, BASE_X + 125, CARC_Y - 30 + TXT_YOFFS, 1200, 30, hWnd, NULL, NULL, NULL);
    texTextC = CreateWindowW(L"STATIC", L"CARC", WS_VISIBLE | WS_CHILD, BASE_X + 125, CARC_Y + TXT_YOFFS, 1200, 30, hWnd, NULL, NULL, NULL);
    HWND hwndButtonCarc = CreateWindowW(L"BUTTON", L"Import Carc", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, CARC_Y, 100, 30, hWnd, (HMENU)CARC_IMPORT, NULL, NULL);
    HWND hwndButtonTrack = CreateWindowW(L"BUTTON", L"Replace", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, CARC_Y + 40, 100, 30, hWnd, (HMENU)FILE_REP, NULL, NULL);

    hwndCBS = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL, BASE_X, TRACKNNAME_Y, 250, 240 /* * TRACKS */, hWnd, NULL, hInst, NULL);
    langCBS = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL, 450, TRACKNNAME_Y, 120, 240, hWnd, NULL, hInst, NULL);

    CreateWindowW(L"STATIC", L"Name :", WS_VISIBLE | WS_CHILD, 325, TRACKNNAME_Y - 26, 80, 20, hWnd, NULL, NULL, NULL);
    hName = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, 325, TRACKNNAME_Y - 2, 100, 26, hWnd, 0, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Rename", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 590, TRACKNNAME_Y - 7, 100, 30, hWnd, (HMENU)RENAME, NULL, NULL);
    musicCBS = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL, BASE_X, TRACKNNAME_Y + 70, 250, 240 /* * TRACKS */, hWnd, NULL, hInst, NULL);
    CreateWindowW(L"STATIC", L"Music :", WS_VISIBLE | WS_CHILD, BASE_X, TRACKNNAME_Y + 45, 120, 25, hWnd, NULL, NULL, NULL);
    bank = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL | ES_NUMBER, BASE_X + 270, TRACKNNAME_Y + 68, 100, 26, hWnd, 0, NULL, NULL);
    CreateWindowW(L"STATIC", L"Sound Effect ID :", WS_VISIBLE | WS_CHILD, BASE_X + 260, TRACKNNAME_Y + 45, 260, 23, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"(Optionnal)", WS_VISIBLE | WS_CHILD, BASE_X + 284, TRACKNNAME_Y + 95, 260, 23, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Modify", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X + 385, TRACKNNAME_Y + 64, 100, 30, hWnd, (HMENU)MUS_REP, NULL, NULL);

    SendMessage(bank, EM_SETLIMITTEXT, 2, NULL);
    SendMessage(hName, EM_SETLIMITTEXT, 25, NULL);

    HWND hwndButtonNCGR = CreateWindowW(L"BUTTON", L"Import NCGR", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, NC_Y, 100, 30, hWnd, (HMENU)NCGR, NULL, NULL);
    HWND hwndButtonNCLR = CreateWindowW(L"BUTTON", L"Import NCLR", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, NC_Y + 40, 100, 30, hWnd, (HMENU)NCLR, NULL, NULL);
    HWND hwndButtonNSCR = CreateWindowW(L"BUTTON", L"Import NSCR", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, NC_Y + 80, 100, 30, hWnd, (HMENU)NSCR, NULL, NULL);
    texTextN = CreateWindowW(L"STATIC", L"NCGR", WS_VISIBLE | WS_CHILD, BASE_X + 125, NC_Y + TXT_YOFFS, 1200, 30, hWnd, NULL, NULL, NULL);
    texTextN2 = CreateWindowW(L"STATIC", L"NCLR", WS_VISIBLE | WS_CHILD, BASE_X + 125, NC_Y + 40 + TXT_YOFFS, 1200, 30, hWnd, NULL, NULL, NULL);
    texTextN3 = CreateWindowW(L"STATIC", L"NSCR", WS_VISIBLE | WS_CHILD, BASE_X + 125, NC_Y + 80 + TXT_YOFFS, 1200, 30, hWnd, NULL, NULL, NULL);
    HWND hwndButtonNC = CreateWindowW(L"BUTTON", L"Change", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X, NC_Y + 125, 100, 30, hWnd, (HMENU)NC_REP, NULL, NULL);

    hTX = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, BASE_X + 30 + OFFSET, MAP_Y, 100, 26, hWnd, 0, NULL, NULL);
    hBX = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, BASE_X + 30 + OFFSET, MAP_Y + 50, 100, 26, hWnd, 0, NULL, NULL);
    hTY = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, BASE_X + 180 + OFFSET, MAP_Y, 100, 26, hWnd, 0, NULL, NULL);
    hBY = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, BASE_X + 180 + OFFSET, MAP_Y + 50, 100, 26, hWnd, 0, NULL, NULL);
    EnforceSignedIntegerEdit(hTX);
    EnforceSignedIntegerEdit(hTY);
    EnforceSignedIntegerEdit(hBX);
    EnforceSignedIntegerEdit(hBY);
    CreateWindowW(L"STATIC", L"Top Left :", WS_VISIBLE | WS_CHILD, BASE_X, MAP_Y + TXT_YOFFS, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"Bottom Right :", WS_VISIBLE | WS_CHILD, BASE_X, MAP_Y + 50 + TXT_YOFFS, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"X :", WS_VISIBLE | WS_CHILD, BASE_X + 30 + OFFSET, MAP_Y - 26, 100, 26, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"Y :", WS_VISIBLE | WS_CHILD, BASE_X + 180 + OFFSET, MAP_Y - 26, 100, 26, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Local Map ? :", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CHECKBOX, BASE_X + 310 + OFFSET, MAP_Y, 120, 30, hWnd, (HMENU)MAP_PUSH, NULL, NULL);
    mapText = CreateWindowW(L"STATIC", L"Global Map", WS_VISIBLE | WS_CHILD, BASE_X + 430 + OFFSET, MAP_Y + TXT_YOFFS, 1200, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Modify", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, BASE_X + 310 + OFFSET, MAP_Y + 46, 100, 30, hWnd, (HMENU)MAP_REP, NULL, NULL);

    for (int i = 0; i < TRACKS; i++)
    {
        SendMessage(hwndCBS, CB_ADDSTRING, 0, TrckNames[i]);
    }
    SendMessage(hwndCBS, CB_SETCURSEL, 0, 0);
    for (int i = 0; i < TRACKSMUS; i++)
    {
        SendMessage(musicCBS, CB_ADDSTRING, 0, TrckMusNames[i]);
    }
    SendMessage(musicCBS, CB_SETCURSEL, 0, 0);
    SendMessageW(langCBS, CB_ADDSTRING, 0, L"English");
    SendMessageW(langCBS, CB_ADDSTRING, 0, L"Italiano");
    SendMessageW(langCBS, CB_ADDSTRING, 0, L"Deutsch");
    SendMessageW(langCBS, CB_ADDSTRING, 0, L"Français");
    SendMessageW(langCBS, CB_ADDSTRING, 0, L"Español");
    SendMessage(langCBS, CB_SETCURSEL, 0, 0);

    MSG msg = {0};

    while (GetMessage((&msg), NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
//
//
//

//
///
//
//
//
//
//
//
//
/*COPIED */
int IsUnicodeDigit(wchar_t ch)
{
    WORD type;
    return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) &&
           (type & C1_DIGIT);
}

LRESULT CALLBACK SignedIntegerSubclassProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
    case WM_NCDESTROY:
        RemoveWindowSubclass(hwnd, SignedIntegerSubclassProc, uIdSubclass);
        break;

    case WM_CHAR:
    {
        wchar_t ch = (wchar_t)wParam;
        // int len = GetWindowTextLength(hwnd);
        int cursor = SendMessage(hwnd, EM_GETSEL, NULL, NULL) & 0x0000FFFF;
        if (ch < L' ')
            break; // let control character through
        if (ch == L'-' && (!cursor))
            break; // let hyphen-minus through
        else if (ch == L'\x2212')
            break; // let Unicode minus sign through
        else if (IsUnicodeDigit(ch))
            break;      // let digit through
        MessageBeep(0); // otherwise invalid
        return 0;
    }
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

BOOL EnforceSignedIntegerEdit(HWND hwnd)
{
    SendMessage(hwnd, EM_SETLIMITTEXT, 6, NULL);
    return SetWindowSubclass(hwnd, SignedIntegerSubclassProc, 0, 0);
}
/*----*/
//
//
//
//
///
//
//
//
//
//
//
//
int preCheck(int BX, int BY, int TX, int TY)
{

    if ((BX > 32767 || BX < -32768) || (BY > 32767 || BY < -32768) || (TX > 32767 || TX < -32768) || (TY > 32767 || TY < -32768))
    {
        return 0;
    }
    else if (!BX && !BY && !TX && !TY)
        return 2;
    return 1;
}
//
//
//
//
///
//
//
//
//
void dealString(char t[4][8])
{
    for (int i = 0; i < 4; i++)
    {
        // printf("%s\n", t[i]);

        if (t[i][0] == '\0')
        {

            t[i][0] = 0;
            t[i][1] = '\0';
        }
        else if (t[i][0] == '-' && t[i][1] == '\0')
        {

            t[i][0] = 0;
            t[i][1] = '\0';
        }
    }
}
//
//

LRESULT CALLBACK WindProce(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int NC__;
    int ba;
    switch (msg)
    {
        /*  case WM_CREATE:

             break; */
    case WM_COMMAND:
        /* if (HIWORD(wp) == CBN_SELCHANGE)
        {

        } */
        switch (wp)
        {

        case TEX_BOX:
            if (isTex)
            {
                isTex = 0;
                SetWindowTextW(texText, L"Not Tex Carc");
            }
            else
            {
                isTex = 1;
                SetWindowTextW(texText, L"Is Tex Carc");
            }

            break;
        case FOLD_BOX:
            if (CarcF)
            {
                fclose(CarcF);
                SetWindowTextA(texTextC, "");
            }
            if (isFold)
            {
                isFold = 0;
                FILE *temp = fopen(TEMP_NAME, "r");
                if (temp)
                {
                    fclose(temp);
                    remove(TEMP_NAME);
                }
            }
            else
            {
                isFold = 1;
            }

            break;
        case MAP_PUSH:
            if (isLocal)
            {
                isLocal = 0;
                SetWindowTextW(mapText, L"Global Map");
            }
            else
            {
                isLocal = 1;
                SetWindowTextW(mapText, L"Local Map");
            }
            break;
        case MAP_REP:
        {
            char temp[4][8];
            GetWindowTextA(hBX, temp[0], 7);

            GetWindowTextA(hBY, temp[1], 7);

            GetWindowTextA(hTX, temp[2], 7);

            GetWindowTextA(hTY, temp[3], 7);

            // printf("%s\n%s\n%s\n%s\n", temp[0], temp[1], temp[2], temp[3]);
            dealString(temp);
            // printf("%s\n%s\n%s\n%s\n", temp[0], temp[1], temp[2], temp[3]);
            int temp2 = preCheck(atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]));
            // printf("%d   %d   %d   %d\n", atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]));

            if (!MkdsF)
            {
                MessageBox(hWnd, "Error, Please import an (EU) Mario Kart DS Rom", "Error", MB_ICONERROR);
                goto brk;
            }
            if (!checkDecomp(MkdsF))
            {
                int ans = MessageBox(hWnd, "Please decompress your ROM's ARM9 to proceed.", "Hey !", MB_ICONQUESTION | MB_YESNO | MB_APPLMODAL);
                if (ans == IDYES)
                {

                    decompArm9ToRom(MkdsF, hWnd);
                    MessageBox(hWnd, "Successfully decompressed (normally)!", "Success", MB_ICONINFORMATION);
                }
                else
                    goto brk;
            }

            if (!temp2)
            {
                MessageBox(hWnd, "Error, coordinates must be between -32 768 and 32 767.", "Error", MB_ICONERROR);
                goto brk;
            }
            else if (temp2 == 2)
            {
                MessageBox(hWnd, "Empty or only 0 coordinates aren't accepted. Check your inputs.", "Warning", MB_ICONWARNING);
                goto brk;
            }

            if (!LocGlobRepl(atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]), isLocal))

                MessageBox(hWnd, "Error modifying the coordinates", "Error", MB_ICONERROR);

            else
                MessageBox(hWnd, "Success Normally !", "Success", MB_ICONINFORMATION);
        brk:
            break;
        }
        case RENAME:
            if (!MkdsF)
            {
                MessageBox(hWnd, "Error, Please import an (EU) Mario Kart DS Rom", "Error", MB_ICONERROR);
                goto brk2;
            }
            unsigned int length = GetWindowTextLength(hName);
            if (!length)
            {
                MessageBox(hWnd, "Error, Please input a name.", "Error", MB_ICONERROR);
                goto brk2;
            }
            char *temp = malloc(length + 2);
            GetWindowTextA(hName, temp, length + 1);
            if (!nameReplacing(temp, length))
            {
                MessageBox(hWnd, "Error renaming. Please Retry..", "Error", MB_ICONERROR);
            }
            else
                MessageBox(hWnd, "Success Normally !", "Success", MB_ICONINFORMATION);
            free(temp);
        brk2:
            break;
        case MKDS_IMPORT:
            /* SetWindowTextA(texTextN, openerNCGR.lpstrFile);
            SetWindowTextA(texTextN2, openerNCLR.lpstrFile);
            SetWindowTextA(texTextN3, openerNSCR.lpstrFile); */
            ba = dialogForCarc(MKDS);
            if (!ba)
            {

                openerM.lpstrFile = "";
                // MessageBox(hWnd, "Error importing the MKDS Rom. Please retry", "Error importing the MKDS Rom", MB_ICONERROR);
            }
            else if (ba != 2)
            {
                SetWindowTextA(texTextM, openerM.lpstrFile);
                if (!checkDecomp(MkdsF))
                {
                    int ans = MessageBox(hWnd, "Would you like to decompress the arm9 in the rom ?\nIt will help the program for bottom screen maps.", "Hey !", MB_ICONQUESTION | MB_YESNO | MB_APPLMODAL);
                    if (ans == IDYES)
                    {

                        decompArm9ToRom(MkdsF, hWnd);
                        MessageBox(hWnd, "Successfully decompressed (normally)!", "Success", MB_ICONINFORMATION);
                    }
                }
            }
            break;

        case CARC_IMPORT:
            ba = dialogForCarc(CARC);
            if (!ba)
            {
                openerM.lpstrFile = "";
                // MessageBox(hWnd, "Error importing the Carc. Please retry", "Error importing the Carc", MB_ICONERROR);
            }
            else if (ba != 2)
            {
                if (!isFold)
                    SetWindowTextA(texTextC, openerC.lpstrFile);
            }
            break;

        case FILE_REP:
            if (!MkdsF)
            {
                MessageBox(hWnd, "Error, Please import an (EU) Mario Kart DS Rom", "Error", MB_ICONERROR);
                goto brk5;
            }
            ba = trackReplacing();
            if (!ba)
                MessageBox(hWnd, "Error Replacing the Track.\nEither the Carc is missing or an unknown issue occured.\nPlease retry", "Error Replacing", MB_ICONERROR);
            else if (ba == 1)
            {
                if (isTex)
                {
                    MessageBox(hWnd, "Success Normally !\nRemember to add the Course Model Carc if not done !", "Success", MB_ICONINFORMATION);

                    SetWindowTextA(texTextC, openerC.lpstrFile);
                }
                else
                {

                    SetWindowTextA(texTextC, openerC.lpstrFile);
                    MessageBox(hWnd, "Success Normally !\nRemember to add the Texture Carc if not done !", "Success", MB_ICONINFORMATION);
                }
            }
        brk5:
            break;

        case NCGR:
            NC__ = 0;
            goto NCC;
        case NCLR:
            NC__ = 1;
            goto NCC;
        case NSCR:
            NC__ = 2;
        NCC:
            ba = dialogForNC(NC__);
            if (!ba)
            {
                MessageBox(hWnd, "Error importing\nPlease retry", "Error", MB_ICONERROR);
            }
            else if (ba != 2)
            {
                switch (NC__)
                {
                case 0:
                    SetWindowTextA(texTextN, openerNCGR.lpstrFile);
                    break;
                case 1:
                    SetWindowTextA(texTextN2, openerNCLR.lpstrFile);
                    break;

                default:
                    SetWindowTextA(texTextN3, openerNSCR.lpstrFile);
                    break;
                }
            }

            break;
        case NC_REP:
            int a = SendMessage(hwndCBS, CB_GETCURSEL, 0, 0);
            if (!MkdsF)
            {
                MessageBox(hWnd, "Error, Please import an (EU) Mario Kart DS Rom", "Error", MB_ICONERROR);
                goto brk4;
            }
            if (a < TRACKS - 6)
            {
                if (!NCReplacing(a))
                {

                    MessageBox(hWnd, "Error Replacing the Pics.\nPlease check the inputs and retry", "Error Replacing", MB_ICONERROR);
                }
                else
                {
                    SetWindowTextA(texTextN, openerNCGR.lpstrFile);
                    SetWindowTextA(texTextN2, openerNCLR.lpstrFile);
                    SetWindowTextA(texTextN3, openerNSCR.lpstrFile);
                    MessageBox(hWnd, "Success Normally !\nPics Changed !", "Success", MB_ICONINFORMATION);
                }
            }
            else
            {
                MessageBox(hWnd, "Error .\nBattle Pics Not Supported", "Error", MB_ICONERROR);
            }
        brk4:
            break;

        case MUS_REP:
            if (!MkdsF)
            {
                MessageBox(hWnd, "Error, Please import an (EU) Mario Kart DS Rom", "Error", MB_ICONERROR);
                goto brk3;
            }
            if (!checkDecomp(MkdsF))
            {
                int ans = MessageBox(hWnd, "Please decompress your ROM's ARM9 to proceed.", "Hey !", MB_ICONQUESTION | MB_YESNO | MB_APPLMODAL);
                if (ans == IDYES)
                {

                    decompArm9ToRom(MkdsF, hWnd);
                    MessageBox(hWnd, "Successfully decompressed (normally)!", "Success", MB_ICONINFORMATION);
                }
                else
                    goto brk3;
            }
            char bankID[3];
            GetWindowTextA(bank, bankID, 3);
            int TrackRepl = SendMessage(hwndCBS, CB_GETCURSEL, 0, 0);
            int MusRepl = SendMessage(musicCBS, CB_GETCURSEL, 0, 0);
            switch (musRepl(TrackRepl, MusRepl, atoi(bankID)))
            {
            case 2:
                MessageBox(hWnd, "Battle Track Music not supported", "Error", MB_ICONERROR);
                break;
            case 1:
                MessageBox(hWnd, "Success Normally !", "Success", MB_ICONINFORMATION);
                break;
            default:
                MessageBox(hWnd, "Invalid Sound Effect ID", "Error", MB_ICONERROR);
                break;
            }

        brk3:
            break;
        }
        break;

    case WM_DESTROY:
        if (MkdsF)
            fclose(MkdsF);
        if (CarcF)

            fclose(CarcF);

        FILE *temp = fopen(TEMP_NAME, "r");
        if (temp)
        {
            fclose(temp);
            remove(TEMP_NAME);
        }
        if (ncgr)
            fclose(ncgr);
        if (nclr)
            fclose(nclr);
        if (nscr)
            fclose(nscr);

        PostQuitMessage(0);
        break;

    default:

        return DefWindowProcW(hWnd, msg, wp, lp);
    }
}