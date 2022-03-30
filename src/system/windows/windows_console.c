#include "color.h"
#include "shared.h"
#include "common.h"
#include "windows_local.h"
#include "cvar/cvar.h"
#include "system/system.h"
#include "stringlib.h"

#define SYSCON_DEFAULT_WIDTH 540
#define SYSCON_DEFAULT_HEIGHT 450

#define COPY_ID         1
#define QUIT_ID         2
#define CLEAR_ID        3

#define ERRORBOX_ID     10
#define ERRORTEXT_ID    11

#define EDIT_ID         100
#define INPUT_ID        101

struct WinConData {
    HWND hWnd;
    HWND hwndBuffer;

    HWND hwndButtonClear;
    HWND hwndButtonCopy;
    HWND hwndButtonQuit;

    HWND hwndErrorBox;
    HWND hwndErrorText;

    HBITMAP hbmLogo;
    HBITMAP hbmClearBitmap;

    HBRUSH hbrEditBackground;
    HBRUSH hbrErrorBackground;

    HFONT hfBufferFont;
    HFONT hfButtonFont;

    HWND hwndInputLine;

    char errorString[80];

    char consoleText[512];
    char returnedText[512];
    int visLevel;
    bool quitOnClose;
    int windowWidth;
    int windowHeight;

    WNDPROC SysInputLineWndProc;
};

static struct WinConData s_wcd;

static LONG WINAPI ConWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                              LPARAM lParam)
{
    char *cmdString;
    static bool s_timePolarity;
    int cx, cy;
    float sx, sy;
    float x, y, w, h;

    switch (uMsg) {
    case WM_SIZE:
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);

        sx = (float) cx / SYSCON_DEFAULT_WIDTH;
        sy = (float) cy / SYSCON_DEFAULT_HEIGHT;

        x = 5;
        y = 40;
        w = cx - 15;
        h = cy - 100;
        SetWindowPos(s_wcd.hwndBuffer, NULL, x, y, w, h, 0);

        y = y + h + 8;
        h = 20;
        SetWindowPos(s_wcd.hwndInputLine, NULL, x, y, w, h, 0);

        y = y + h + 4;
        w = 72 * sx;
        h = 24;
        SetWindowPos(s_wcd.hwndButtonCopy, NULL, x, y, w, h, 0);

        x = x + w + 2;
        SetWindowPos(s_wcd.hwndButtonClear, NULL, x, y, w, h, 0);

        x = cx - 15 - w;
        SetWindowPos(s_wcd.hwndButtonQuit, NULL, x, y, w, h, 0);

        s_wcd.windowWidth = cx;
        s_wcd.windowHeight = cy;
        break;
    case WM_ACTIVATE:
        if (LOWORD(wParam) != WA_INACTIVE)
            SetFocus(s_wcd.hwndInputLine);

        if (com_viewlog && (com_dedicated && !com_dedicated->integer)) {
            // if the viewlog is open, check to see if it's being minimized
            if (com_viewlog->integer == 1) {
                if (HIWORD(wParam)) {   // minimized flag
                    cvar_set("viewlog", "2");
                }
            } else if (com_viewlog->integer == 2) {
                if (!HIWORD(wParam)) {      // minimized flag
                    cvar_set("viewlog", "1");
                }
            }
        }
        break;

    case WM_CLOSE:
        if ((com_dedicated && com_dedicated->integer)) {
            cmdString = strdup("quit");
            sys_queue_event(0, SE_CONSOLE, 0, 0, strlen(cmdString) + 1, cmdString);
        } else if (s_wcd.quitOnClose) {
            PostQuitMessage(0);
        } else {
            console_show(0, false);
            cvar_set("viewlog", "0");
        }
        return 0;
    case WM_CTLCOLORSTATIC:
        if ((HWND) lParam == s_wcd.hwndBuffer) {
            SetBkColor((HDC) wParam, RGB(86, 117, 100));
            SetTextColor((HDC) wParam, RGB(0xff, 0xff, 0xff));
            return (long) s_wcd.hbrEditBackground;
        } else if ((HWND) lParam == s_wcd.hwndErrorBox) {
            if (s_timePolarity & 1) {
                SetBkColor((HDC) wParam, RGB(0x80, 0x80, 0x80));
                SetTextColor((HDC) wParam, RGB(0xff, 0x0, 0x00));
            } else {
                SetBkColor((HDC) wParam, RGB(0x80, 0x80, 0x80));
                SetTextColor((HDC) wParam, RGB(0x00, 0x0, 0x00));
            }
            return (long) s_wcd.hbrErrorBackground;
        }
        break;

    case WM_COMMAND:
        if (wParam == COPY_ID) {
            SendMessage(s_wcd.hwndBuffer, EM_SETSEL, 0, -1);
            SendMessage(s_wcd.hwndBuffer, WM_COPY, 0, 0);
        } else if (wParam == QUIT_ID) {
            if (s_wcd.quitOnClose) {
                PostQuitMessage(0);
            } else {
                cmdString = strdup("quit");
                sys_queue_event(0, SE_CONSOLE, 0, 0, strlen(cmdString) + 1, cmdString);
            }
        } else if (wParam == CLEAR_ID) {
            SendMessage(s_wcd.hwndBuffer, EM_SETSEL, 0, -1);
            SendMessage(s_wcd.hwndBuffer, EM_REPLACESEL, FALSE, (LPARAM) "");
            UpdateWindow(s_wcd.hwndBuffer);
        }
        break;
    case WM_CREATE:
        s_wcd.hbrEditBackground = CreateSolidBrush(RGB(86, 117, 100));
        s_wcd.hbrErrorBackground = CreateSolidBrush(RGB(0x80, 0x80, 0x80));
        SetTimer(hWnd, 1, 1000, NULL);
        break;
    case WM_ERASEBKGND:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    case WM_TIMER:
        if (wParam == 1) {
            s_timePolarity = !s_timePolarity;
            if (s_wcd.hwndErrorBox) {
                InvalidateRect(s_wcd.hwndErrorBox, NULL, FALSE);
            }
        }
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LONG WINAPI InputLineWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                             LPARAM lParam)
{
    char inputBuffer[1024];

    switch (uMsg)
    {
    case WM_KILLFOCUS:
        if ((HWND) wParam == s_wcd.hWnd ||
            (HWND) wParam == s_wcd.hwndErrorBox) {
            SetFocus(hWnd);
            return 0;
        }
        break;

    case WM_CHAR:
        if (wParam == 13) {
            GetWindowText(s_wcd.hwndInputLine, inputBuffer, sizeof(inputBuffer));
            strncat(s_wcd.consoleText, inputBuffer, 
                    sizeof(s_wcd.consoleText) - strlen(s_wcd.consoleText) - 5);
            strcat(s_wcd.consoleText, "\n");
            SetWindowText(s_wcd.hwndInputLine, "");

            sys_print(va("]%s\n", inputBuffer));

            return 0;
        }
    }

    return CallWindowProc(s_wcd.SysInputLineWndProc, hWnd, uMsg, wParam, lParam);

}

void console_create(void)
{
    HDC hDC;
    WNDCLASS wc;
    RECT rect;
    const char *DEDCLASS = "iocod";
    const char *WINDOWNAME = "iocod console";

    int nHeight, swidth, sheight;
    int DEDSTYLE = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX;

    memset(&wc, 0, sizeof(wc));

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC) ConWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_wv.hInstance;
    //wc.hIcon = LoadIcon(g_wv.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (void *) COLOR_WINDOW;
    wc.lpszMenuName = 0;
    wc.lpszClassName = DEDCLASS;

    if (!RegisterClass(&wc))
        return;

    rect.left = 0;
    rect.right = SYSCON_DEFAULT_WIDTH;
    rect.top = 0;
    rect.bottom = SYSCON_DEFAULT_HEIGHT;
    AdjustWindowRect(&rect, DEDSTYLE, FALSE);

    hDC = GetDC(GetDesktopWindow());
    swidth = GetDeviceCaps(hDC, HORZRES);
    sheight = GetDeviceCaps(hDC, VERTRES);
    ReleaseDC(GetDesktopWindow(), hDC);

    s_wcd.windowWidth = rect.right - rect.left + 1;
    s_wcd.windowHeight = rect.bottom - rect.top + 1;

    s_wcd.hWnd = CreateWindowEx(0,
                                DEDCLASS,
                                WINDOWNAME,
                                DEDSTYLE,
                                (swidth - 600) / 2, 
                                (sheight - 450) / 2, 
                                rect.right - rect.left + 1, 
                                rect.bottom - rect.top + 1,
                                NULL,
                                NULL,
                                g_wv.hInstance,
                                NULL);

    if (s_wcd.hWnd == NULL)
        return;

    //
    // create fonts
    //
    hDC = GetDC(s_wcd.hWnd);
    nHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);

    s_wcd.hfBufferFont = CreateFont(nHeight,
                                    0,
                                    0,
                                    0,
                                    FW_LIGHT,
                                    0,
                                    0,
                                    0,
                                    DEFAULT_CHARSET,
                                    OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY,
                                    FF_MODERN | FIXED_PITCH,
                                    "Courier New");

    ReleaseDC(s_wcd.hWnd, hDC);

    //
    // create the input line
    //
    s_wcd.hwndInputLine = CreateWindow("edit", 
                                       NULL, 
                                       WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                                       6, 
                                       400, 
                                       528, 
                                       20,
                                       s_wcd.hWnd,
                                       (HMENU) INPUT_ID,         // child window ID
                                       g_wv.hInstance, NULL);

   //
   // create the buttons
   //
    s_wcd.hwndButtonCopy = CreateWindow("button", 
                                        NULL, 
                                        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                        5, 
                                        425, 
                                        72, 
                                        24,
                                        s_wcd.hWnd,
                                        (HMENU) COPY_ID,         // child window ID
                                        g_wv.hInstance, NULL);
    SendMessage(s_wcd.hwndButtonCopy, WM_SETTEXT, 0, (LPARAM) "copy");

    s_wcd.hwndButtonClear = CreateWindow("button", 
                                         NULL, 
                                         BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                         82, 
                                         425, 
                                         72, 
                                         24,
                                         s_wcd.hWnd,
                                         (HMENU) CLEAR_ID,       // child window ID
                                         g_wv.hInstance, NULL);
    SendMessage(s_wcd.hwndButtonClear, WM_SETTEXT, 0, (LPARAM) "clear");

    s_wcd.hwndButtonQuit = CreateWindow("button", 
                                        NULL, 
                                        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                        462, 
                                        425, 
                                        72, 
                                        24,
                                        s_wcd.hWnd,
                                        (HMENU) QUIT_ID,         // child window ID
                                        g_wv.hInstance, NULL);
    SendMessage(s_wcd.hwndButtonQuit, WM_SETTEXT, 0, (LPARAM) "quit");


    //
    // create the scrollbuffer
    //
    s_wcd.hwndBuffer = CreateWindow("edit", 
                                    NULL, 
                                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                                    6, 
                                    40, 
                                    526, 
                                    354,
                                    s_wcd.hWnd,
                                    (HMENU) EDIT_ID,             // child window ID
                                    g_wv.hInstance, NULL);
    SendMessage(s_wcd.hwndBuffer, WM_SETFONT, (WPARAM) s_wcd.hfBufferFont, 0);

    s_wcd.SysInputLineWndProc = (WNDPROC) SetWindowLong(s_wcd.hwndInputLine, 
                                                        GWLP_WNDPROC, 
                                                        (long) InputLineWndProc);
    SendMessage(s_wcd.hwndInputLine, WM_SETFONT, (WPARAM) s_wcd.hfBufferFont, 0);

    ShowWindow(s_wcd.hWnd, SW_SHOWDEFAULT);
    UpdateWindow(s_wcd.hWnd);
    SetForegroundWindow(s_wcd.hWnd);
    SetFocus(s_wcd.hwndInputLine);

    s_wcd.visLevel = 1;
}

void console_destroy(void)
{
    if (s_wcd.hWnd) {
        ShowWindow(s_wcd.hWnd, SW_HIDE);
        CloseWindow(s_wcd.hWnd);
        DestroyWindow(s_wcd.hWnd);
        s_wcd.hWnd = 0;
    }
}

void console_show(int visLevel, bool quitOnClose)
{
    s_wcd.quitOnClose = quitOnClose;

    if (visLevel == s_wcd.visLevel)
        return;

    s_wcd.visLevel = visLevel;

    if (!s_wcd.hWnd)
        return;

    switch (visLevel)
    {
    case 0:
        ShowWindow(s_wcd.hWnd, SW_HIDE);
        break;
    case 1:
        ShowWindow(s_wcd.hWnd, SW_SHOWNORMAL);
        SendMessage(s_wcd.hwndBuffer, EM_LINESCROLL, 0, 0xffff);
        break;
    case 2:
        ShowWindow(s_wcd.hWnd, SW_MINIMIZE);
        break;
    default:
        sys_error("Invalid visLevel %d sent to Sys_ShowConsole\n", visLevel);
        break;
    }
}

void console_append(const char *pMsg)
{
    #define CONSOLE_BUFFER_SIZE     16384

    char buffer[CONSOLE_BUFFER_SIZE * 2];
    char *b = buffer;
    const char *msg;
    int bufLen;
    int i = 0;
    static unsigned long s_totalChars;

    //
    // if the message is REALLY long, use just the last portion of it
    //
    if (strlen(pMsg) > CONSOLE_BUFFER_SIZE - 1) {
        msg = pMsg + strlen(pMsg) - CONSOLE_BUFFER_SIZE + 1;
    } else
    {
        msg = pMsg;
    }

    //
    // copy into an intermediate buffer
    //
    while (msg[i] && ((b - buffer) < sizeof(buffer) - 1))
    {
        if (msg[i] == '\n' && msg[i + 1] == '\r') {
            b[0] = '\r';
            b[1] = '\n';
            b += 2;
            i++;
        } else if (msg[i] == '\r') {
            b[0] = '\r';
            b[1] = '\n';
            b += 2;
        } else if (msg[i] == '\n') {
            b[0] = '\r';
            b[1] = '\n';
            b += 2;
        } else if (is_color_string(&msg[i])) {
            i++;
        } else
        {
            *b = msg[i];
            b++;
        }
        i++;
    }
    *b = 0;
    bufLen = b - buffer;

    s_totalChars += bufLen;

    //
    // replace selection instead of appending if we're overflowing
    //
    if (s_totalChars > CONSOLE_BUFFER_SIZE) {
        SendMessage(s_wcd.hwndBuffer, EM_SETSEL, 0, -1);
        s_totalChars = bufLen;
    } else {
        // NERVE - SMF - always append at the bottom of the textbox
        SendMessage(s_wcd.hwndBuffer, EM_SETSEL, 0xFFFF, 0xFFFF);
    }

    //
    // put this text into the windows console
    //
    SendMessage(s_wcd.hwndBuffer, EM_LINESCROLL, 0, 0xffff);
    SendMessage(s_wcd.hwndBuffer, EM_SCROLLCARET, 0, 0);
    SendMessage(s_wcd.hwndBuffer, EM_REPLACESEL, 0, (LPARAM) buffer);
}

void set_error_text(const char *buf)
{
    q_strncpyz(s_wcd.errorString, buf, sizeof(s_wcd.errorString));

    if (!s_wcd.hwndErrorBox) {
        s_wcd.hwndErrorBox = CreateWindow("static", NULL, WS_CHILD | WS_VISIBLE | SS_SUNKEN,
                                          6, 
                                          5, 
                                          526, 
                                          30,
                                          s_wcd.hWnd,
                                          (HMENU) ERRORBOX_ID,           // child window ID
                                          g_wv.hInstance, NULL);
        SendMessage(s_wcd.hwndErrorBox, WM_SETFONT, (WPARAM) s_wcd.hfBufferFont, 0);
        SetWindowText(s_wcd.hwndErrorBox, s_wcd.errorString);

        DestroyWindow(s_wcd.hwndInputLine);
        s_wcd.hwndInputLine = NULL;
    }
}

void sys_console_input_shutdown(void)
{

}

void sys_console_input_init(void)
{

}

char *sys_console_input(void)
{

}

void tty_hide(void)
{

}

void tty_show(void)
{

}
