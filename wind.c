
#include "FileAdd.h"
#include "putFile.h"
#include "carcPut.h"
#include "windowST.h"

HWND texText, texTextM, texTextC, texTextN, texTextN2, texTextN3, mapText, hBX, hBY, hTX, hTY;
char isLocal;
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

#define OFFSET 100 // Kinda hacky or smth but hey it modifies faster the x pos of the map stufff

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

    if (!(RegisterClassW(&wc)))
        return -1;

    hWnd = CreateWindowW(L"windo", L"Track Replacer 0.0.1", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1000, 700, NULL, NULL, NULL, NULL);

    HWND hwndButtonTex = CreateWindowW(L"BUTTON", L"Is Tex Carc ? :", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CHECKBOX, 50, 25, 120, 30, hWnd, (HMENU)TEX_BOX, NULL, NULL);

    texText = CreateWindowW(L"STATIC", L"Not Tex Carc", WS_VISIBLE | WS_CHILD, 175, 31, 1200, 30, hWnd, NULL, NULL, NULL);

    texTextM = CreateWindowW(L"STATIC", L"MKDS", WS_VISIBLE | WS_CHILD, 175, 170, 1200, 30, hWnd, NULL, NULL, NULL);

    texTextC = CreateWindowW(L"STATIC", L"CARC", WS_VISIBLE | WS_CHILD, 175, 100, 1200, 30, hWnd, NULL, NULL, NULL);

    HWND hwndButtonCarc = CreateWindowW(L"BUTTON", L"Import Carc", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 95, 100, 30, hWnd, (HMENU)CARC_IMPORT, NULL, NULL);

    HWND hwndButtonMkds = CreateWindowW(L"BUTTON", L"Import MKDS", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 165, 100, 30, hWnd, (HMENU)MKDS_IMPORT, NULL, NULL);
    HWND hwndButtonTrack = CreateWindowW(L"BUTTON", L"Replace", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 215, 100, 30, hWnd, (HMENU)FILE_REP, NULL, NULL);
    hwndCBS = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL, 50, 60, 250, 240 /* * TRACKS */, hWnd, NULL, hInst, NULL);
    langCBS = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_OVERLAPPED | WS_VSCROLL, 325, 60, 120, 240, hWnd, NULL, hInst, NULL);

    //

    HWND hwndButtonNCGR = CreateWindowW(L"BUTTON", L"Import NCGR", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 295, 100, 30, hWnd, (HMENU)NCGR, NULL, NULL);
    HWND hwndButtonNCLR = CreateWindowW(L"BUTTON", L"Import NCLR", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 335, 100, 30, hWnd, (HMENU)NCLR, NULL, NULL);
    HWND hwndButtonNSCR = CreateWindowW(L"BUTTON", L"Import NSCR", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 375, 100, 30, hWnd, (HMENU)NSCR, NULL, NULL);
    texTextN = CreateWindowW(L"STATIC", L"NCGR", WS_VISIBLE | WS_CHILD, 175, 300, 1200, 30, hWnd, NULL, NULL, NULL);

    texTextN2 = CreateWindowW(L"STATIC", L"NCLR", WS_VISIBLE | WS_CHILD, 175, 340, 1200, 30, hWnd, NULL, NULL, NULL);

    texTextN3 = CreateWindowW(L"STATIC", L"NSCR", WS_VISIBLE | WS_CHILD, 175, 380, 1200, 30, hWnd, NULL, NULL, NULL);
    HWND hwndButtonNC = CreateWindowW(L"BUTTON", L"Change", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 50, 420, 100, 30, hWnd, (HMENU)NC_REP, NULL, NULL);
    hTX = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, 80 + OFFSET, 520, 100, 26, hWnd, 0, NULL, NULL);
    hBX = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, 80 + OFFSET, 570, 100, 26, hWnd, 0, NULL, NULL);
    hTY = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, 230 + OFFSET, 520, 100, 26, hWnd, 0, NULL, NULL);
    hBY = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_DLGFRAME | ES_AUTOHSCROLL, 230 + OFFSET, 570, 100, 26, hWnd, 0, NULL, NULL);
    EnforceSignedIntegerEdit(hTX);
    EnforceSignedIntegerEdit(hTY);
    EnforceSignedIntegerEdit(hBX);
    EnforceSignedIntegerEdit(hBY);
    CreateWindowW(L"STATIC", L"Top Left :", WS_VISIBLE | WS_CHILD, 50, 525, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"Bottom Right :", WS_VISIBLE | WS_CHILD, 50, 575, 100, 30, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"X :", WS_VISIBLE | WS_CHILD, 80 + OFFSET, 494, 100, 26, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"Y :", WS_VISIBLE | WS_CHILD, 230 + OFFSET, 494, 100, 26, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Local Map ? :", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_CHECKBOX, 360 + OFFSET, 520, 120, 30, hWnd, (HMENU)MAP_PUSH, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Modify", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT, 360 + OFFSET, 566, 100, 30, hWnd, (HMENU)MAP_REP, NULL, NULL);
    mapText = CreateWindowW(L"STATIC", L"Global Map", WS_VISIBLE | WS_CHILD, 480 + OFFSET, 526, 1200, 30, hWnd, NULL, NULL, NULL);

    for (int i = 0; i < TRACKS; i++)
    {
        SendMessage(hwndCBS, CB_ADDSTRING, 0, TrckNames[i]);
    }
    SendMessage(hwndCBS, CB_SETCURSEL, 0, 0);
    SendMessage(langCBS, CB_ADDSTRING, 0, "English");
    SendMessage(langCBS, CB_ADDSTRING, 0, "Italiano");
    SendMessage(langCBS, CB_ADDSTRING, 0, "Deutsch");
    SendMessage(langCBS, CB_ADDSTRING, 0, "Français");
    SendMessage(langCBS, CB_ADDSTRING, 0, "Español");
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
void dealString(char t[4][7])
{
    for (int i = 0; i < 4; i++)
    {

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

            dealString(temp);
            // printf("%s\n%s\n%s\n%s\n", temp[0], temp[1], temp[2], temp[3]);
            int temp2 = preCheck(atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]));

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
                MessageBox(hWnd, "Empty strings are treated as 0s. Check your inputs.", "Warning", MB_ICONWARNING);

            if (!LocGlobRepl(atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]), isLocal))

                MessageBox(hWnd, "Error modifying the coordinates", "Error", MB_ICONERROR);

            else
                MessageBox(hWnd, "Success Normally !", "Success", MB_ICONINFORMATION);
        brk:
            break;
        }
        case MKDS_IMPORT:
            /* SetWindowTextA(texTextN, openerNCGR.lpstrFile);
            SetWindowTextA(texTextN2, openerNCLR.lpstrFile);
            SetWindowTextA(texTextN3, openerNSCR.lpstrFile); */
            if (!dialogForCarc(MKDS))
            {

                openerM.lpstrFile = "";
                MessageBox(hWnd, "Error importing the MKDS Rom. Please retry", "Error importing the MKDS Rom", MB_ICONEXCLAMATION);
            }
            else
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
            if (!dialogForCarc(CARC))
            {
                openerM.lpstrFile = "";
                MessageBox(hWnd, "Error importing the Carc. Please retry", "Error importing the Carc", MB_ICONEXCLAMATION);
            }
            else
            {
                SetWindowTextA(texTextC, openerC.lpstrFile);
            }
            break;

        case FILE_REP:
            if (!trackReplacing())
                MessageBox(hWnd, "Error Replacing the Track.\nEither the Carc missing, the MKDS Rom is missing or an unknown issue occured.\nPlease retry", "Error Replacing", MB_ICONERROR);
            else if (isTex)
            {
                MessageBox(hWnd, "Success Normally !\nRemember to add the Course Model Carc if not done !", "Success", MB_ICONINFORMATION);

                SetWindowTextA(texTextC, openerC.lpstrFile);
            }
            else
            {

                SetWindowTextA(texTextC, openerC.lpstrFile);
                MessageBox(hWnd, "Success Normally !\nRemember to add the Texture Carc if not done !", "Success", MB_ICONINFORMATION);
            }
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
            if (!dialogForNC(NC__))
            {
                MessageBox(hWnd, "Error importing\nPlease retry", "Error", MB_ICONERROR);
            }
            else
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
            break;
        }

        break;
    case WM_DESTROY:
        if (MkdsF)
            fclose(MkdsF);
        if (CarcF)
            fclose(CarcF);
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