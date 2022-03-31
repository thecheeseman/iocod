#include "color.h"
#include "cvar/cvar.h"
#include "system/events.h"
#include "system/shared.h"
#include "system/windows/console.h"
#include "system/windows/local.h"
#include "stringlib.h"

extern struct cvar *com_viewlog;
extern struct cvar *com_dedicated;

#define SYSCON_DEFAULT_WIDTH 540
#define SYSCON_DEFAULT_HEIGHT 450

#define COPY_ID         1
#define QUIT_ID         2
#define CLEAR_ID        3

#define ERRORBOX_ID     10
#define ERRORTEXT_ID    11

#define EDIT_ID         100
#define INPUT_ID        101

#define DEDSTYLE (WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX)
#define EDIT_STYLE (WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL)
#define BUTTON_STYLE (BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON)
#define CONSOLE_STYLE (WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY)
#define ERROR_STYLE (WS_CHILD | WS_VISIBLE | SS_SUNKEN)

struct console_window_data {
    HWND window;
    HWND window_buffer;

    HWND button_clear;
    HWND button_copy;
    HWND button_quit;

    HWND error_box;
    HWND error_text;

    HBITMAP logo;
    HBITMAP clear_bitmap;

    HBRUSH edit_background;
    HBRUSH error_background;

    HFONT buffer_font;
    HFONT button_font;

    HWND input_line;

    char error_string[80];
    char console_text[512];
    char returned_text[512];

    int vis_level;

    bool quit_on_close;

    int width;
    int height;

    WNDPROC input_line_handler;
};

static struct console_window_data console;

static LONG WINAPI console_window_handler(HWND window, UINT message, 
                                          WPARAM param1, LPARAM param2)
{
    char *cmdString;
    static bool s_timePolarity;
    int cx, cy;
    float sx, sy;
    float x, y, w, h;

    switch (message) {
    case WM_SIZE:
        cx = LOWORD(param2);
        cy = HIWORD(param2);

        sx = (float) cx / SYSCON_DEFAULT_WIDTH;
        sy = (float) cy / SYSCON_DEFAULT_HEIGHT;

        x = 5;
        y = 40;
        w = cx - 15;
        h = cy - 100;
        SetWindowPos(console.window_buffer, NULL, x, y, w, h, 0);

        y = y + h + 8;
        h = 20;
        SetWindowPos(console.input_line, NULL, x, y, w, h, 0);

        y = y + h + 4;
        w = 72 * sx;
        h = 24;
        SetWindowPos(console.button_copy, NULL, x, y, w, h, 0);

        x = x + w + 2;
        SetWindowPos(console.button_clear, NULL, x, y, w, h, 0);

        x = cx - 15 - w;
        SetWindowPos(console.button_quit, NULL, x, y, w, h, 0);

        console.width = cx;
        console.height = cy;
        break;
    case WM_ACTIVATE:
        if (LOWORD(param1) != WA_INACTIVE)
            SetFocus(console.input_line);

        if (com_viewlog != NULL && 
            (com_dedicated != NULL && !com_dedicated->integer > 0)) {
            // if the viewlog is open, check to see if it's being minimized
            if (com_viewlog->integer == 1) {
                if (HIWORD(param1))   // minimized flags_wcd.height
                    cvar_set("viewlog", "2");
            } else if (com_viewlog->integer == 2) {
                if (!HIWORD(param1))      // minimized flag
                    cvar_set("viewlog", "1");
            }
        }
        break;

    case WM_CLOSE:
        if ((com_dedicated != NULL && com_dedicated->integer > 0)) {
            cmdString = strdup("quit");
            queue_event(0, SE_CONSOLE, 0, 0, 
                        strlen(cmdString) + 1, cmdString);
        } else if (console.quit_on_close) {
            PostQuitMessage(0);
        } else {
            console_show(0, false);
            cvar_set("viewlog", "0");
        }
        return 0;
    case WM_CTLCOLORSTATIC:
        if ((HWND) param2 == console.window_buffer) {
            SetBkColor((HDC) param1, RGB(86, 117, 100));
            SetTextColor((HDC) param1, RGB(0xff, 0xff, 0xff));
            return (long) console.edit_background;
        } else if ((HWND) param2 == console.error_box) {
            if (s_timePolarity & 1) {
                SetBkColor((HDC) param1, RGB(0x80, 0x80, 0x80));
                SetTextColor((HDC) param1, RGB(0xff, 0x0, 0x00));
            } else {
                SetBkColor((HDC) param1, RGB(0x80, 0x80, 0x80));
                SetTextColor((HDC) param1, RGB(0x00, 0x0, 0x00));
            }
            return (long) console.error_background;
        }
        break;

    case WM_COMMAND:
        if (param1 == COPY_ID) {
            SendMessage(console.window_buffer, EM_SETSEL, 0, -1);
            SendMessage(console.window_buffer, WM_COPY, 0, 0);
        } else if (param1 == QUIT_ID) {
            if (console.quit_on_close) {
                PostQuitMessage(0);
            } else {
                cmdString = strdup("quit");
                queue_event(0, SE_CONSOLE, 0, 0,
                            strlen(cmdString) + 1, cmdString);
            }
        } else if (param1 == CLEAR_ID) {
            SendMessage(console.window_buffer, EM_SETSEL, 0, -1);
            SendMessage(console.window_buffer, EM_REPLACESEL, FALSE, (LPARAM) "");
            UpdateWindow(console.window_buffer);
        }
        break;
    case WM_CREATE:
        console.edit_background = CreateSolidBrush(RGB(86, 117, 100));
        console.error_background = CreateSolidBrush(RGB(0x80, 0x80, 0x80));
        SetTimer(window, 1, 1000, NULL);
        break;
    case WM_ERASEBKGND:
        return DefWindowProc(window, message, param1, param2);
    case WM_TIMER:
        if (param1 == 1) {
            s_timePolarity = !s_timePolarity;
            if (console.error_box)
                InvalidateRect(console.error_box, NULL, FALSE);
        }
        break;
    }

    return DefWindowProc(window, message, param1, param2);
}

static LONG WINAPI input_line_handler(HWND window, UINT message, 
                                      WPARAM param1, LPARAM param2)
{
    char buf[1024];

    switch (message) {
    case WM_KILLFOCUS:
        if ((HWND) param1 == console.window ||
            (HWND) param1 == console.error_box) {
            SetFocus(window);
            return 0;
        }
        break;

    case WM_CHAR:
        if (param1 == 13) {
            GetWindowText(console.input_line, buf, sizeof(buf));

            strncat(console.console_text, buf, 
                    sizeof(console.console_text) - strlen(console.console_text) - 5);
            strcat(console.console_text, "\n");

            SetWindowText(console.input_line, "");

            sys_print(va("]%s\n", buf));

            return 0;
        }
    }

    return CallWindowProc(console.input_line_handler, window, message, 
                          param1, param2);

}

void console_create(void)
{
    HDC hDC;
    WNDCLASS wc;
    RECT rect;
    const char *DEDCLASS = "iocod";
    const char *WINDOWNAME = "iocod console";

    int nHeight, swidth, sheight;

    memset(&wc, 0, sizeof(wc));

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC) console_window_handler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = window_vars.instance;
    //wc.hIcon = LoadIcon(window_vars.instance, MAKEINTRESOURCE(IDI_ICON1));
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

    console.width = rect.right - rect.left + 1;
    console.height = rect.bottom - rect.top + 1;

    console.window = CreateWindowEx(0, DEDCLASS, WINDOWNAME, DEDSTYLE,
                                  (swidth - 600) / 2, (sheight - 450) / 2, 
                                  rect.right - rect.left + 1, 
                                  rect.bottom - rect.top + 1,
                                  NULL, NULL, window_vars.instance, NULL);

    if (console.window == NULL)
        return;

    // create fonts
    hDC = GetDC(console.window);
    nHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);

    console.buffer_font = CreateFont(nHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                     FF_MODERN | FIXED_PITCH, "Courier New");

    ReleaseDC(console.window, hDC);

    // create the input line
    console.input_line = CreateWindow("edit", NULL, EDIT_STYLE, 
                                      6, 400, 528, 20, console.window,
                                      (HMENU) INPUT_ID,
                                      window_vars.instance, NULL);

    // create the buttons
    console.button_copy = CreateWindow("button", NULL, BUTTON_STYLE,
                                       5, 425, 72, 24, console.window,
                                       (HMENU) COPY_ID,
                                       window_vars.instance, NULL);
    SendMessage(console.button_copy, WM_SETTEXT, 0, (LPARAM) "copy");

    console.button_clear = CreateWindow("button", NULL, BUTTON_STYLE,
                                        82, 425, 72, 24, console.window,
                                        (HMENU) CLEAR_ID,
                                        window_vars.instance, NULL);
    SendMessage(console.button_clear, WM_SETTEXT, 0, (LPARAM) "clear");

    console.button_quit = CreateWindow("button", NULL, BUTTON_STYLE,
                                       462, 425, 72, 24, console.window,
                                       (HMENU) QUIT_ID,
                                       window_vars.instance, NULL);
    SendMessage(console.button_quit, WM_SETTEXT, 0, (LPARAM) "quit");


    // create the scrollbuffer
    console.window_buffer = CreateWindow("edit", NULL, CONSOLE_STYLE,
                                         6, 40, 526, 354, console.window,
                                         (HMENU) EDIT_ID,
                                         window_vars.instance, NULL);
    SendMessage(console.window_buffer, WM_SETFONT, (WPARAM) console.buffer_font, 0);

    console.input_line_handler = (WNDPROC) SetWindowLong(console.input_line, 
                                                         GWLP_WNDPROC, 
                                                         (long) input_line_handler);
    SendMessage(console.input_line, WM_SETFONT, (WPARAM) console.buffer_font, 0);

    ShowWindow(console.window, SW_SHOWDEFAULT);
    UpdateWindow(console.window);
    SetForegroundWindow(console.window);
    SetFocus(console.input_line);

    console.vis_level = 1;
}

void console_destroy(void)
{
    if (console.window != NULL) {
        ShowWindow(console.window, SW_HIDE);
        CloseWindow(console.window);
        DestroyWindow(console.window);
        console.window = 0;
    }
}

void console_show(int vis_level, bool quit_on_close)
{
    console.quit_on_close = quit_on_close;

    if (vis_level == console.vis_level)
        return;

    console.vis_level = vis_level;

    if (console.window == NULL)
        return;

    switch (vis_level) {
    case 0:
        ShowWindow(console.window, SW_HIDE);
        break;
    case 1:
        ShowWindow(console.window, SW_SHOWNORMAL);
        SendMessage(console.window_buffer, EM_LINESCROLL, 0, 0xffff);
        break;
    case 2:
        ShowWindow(console.window, SW_MINIMIZE);
        break;
    default:
        sys_error("Invalid visLevel %d sent to Sys_ShowConsole\n", vis_level);
        break;
    }
}

void console_append(const char *message)
{
    #define CONSOLE_BUFFER_SIZE     16384

    char buffer[CONSOLE_BUFFER_SIZE * 2];
    char *b = buffer;
    const char *msg;
    int bufLen;
    int i = 0;
    static unsigned long total;

    // if the message is REALLY long, use just the last portion of it
    if (strlen(message) > CONSOLE_BUFFER_SIZE - 1)
        msg = message + strlen(message) - CONSOLE_BUFFER_SIZE + 1;
    else
        msg = message;

    // copy into an intermediate buffer
    while (msg[i] && ((b - buffer) < sizeof(buffer) - 1)) {
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
        } else {
            *b = msg[i];
            b++;
        }
        i++;
    }
    *b = 0;
    bufLen = b - buffer;

    total += bufLen;

    // replace selection instead of appending if we're overflowing
    if (total > CONSOLE_BUFFER_SIZE) {
        SendMessage(console.window_buffer, EM_SETSEL, 0, -1);
        total = bufLen;
    } else {
        // NERVE - SMF - always append at the bottom of the textbox
        SendMessage(console.window_buffer, EM_SETSEL, 0xFFFF, 0xFFFF);
    }

    // put this text into the windows console
    SendMessage(console.window_buffer, EM_LINESCROLL, 0, 0xffff);
    SendMessage(console.window_buffer, EM_SCROLLCARET, 0, 0);
    SendMessage(console.window_buffer, EM_REPLACESEL, 0, (LPARAM) buffer);
}

void set_error_text(const char *buf)
{
    q_strncpyz(console.error_string, buf, sizeof(console.error_string));

    if (console.error_box == NULL) {
        console.error_box = CreateWindow("static", NULL, ERROR_STYLE,
                                         6, 5, 526, 30, console.window,
                                         (HMENU) ERRORBOX_ID,
                                         window_vars.instance, NULL);
        SendMessage(console.error_box, WM_SETFONT, (WPARAM) console.buffer_font, 0);
        SetWindowText(console.error_box, console.error_string);

        DestroyWindow(console.input_line);
        console.input_line = NULL;
    }
}

char *console_input(void)
{
    if (*console.console_text == '\0')
        return NULL;

    strncpy(console.returned_text, console.console_text, 
            sizeof(console.returned_text));
    console.console_text[0] = '\0';

    return console.returned_text;
}
