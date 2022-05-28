#include "console.h"

#ifdef IC_PLATFORM_WINDOWS
/* windows GUI stuff is abysmal */
void con_create(void)
{
    HDC hDC;
    WNDCLASS wc;
    RECT rect;
    const char *DEDCLASS = "iocod";
    const char *WINDOWNAME = "iocod console";

    int nheight, swidth, sheight;

    memset(&wc, 0, sizeof(wc));

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC) console_window_handler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = window_vars.instance;
    /* wc.hIcon = LoadIcon(window_vars.instance, MAKEINTRESOURCE(IDI_ICON1)); */
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

    /* create fonts */
    hDC = GetDC(console.window);
    nHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);

    console.buffer_font = CreateFont(nheight, 0, 0, 0, FW_LIGHT, 0, 0, 0,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                     FF_MODERN | FIXED_PITCH, "Consolas");

    ReleaseDC(console.window, hDC);

    /* create the input line */
    console.input_line = CreateWindow("edit", NULL, EDIT_STYLE,
                                      6, 400, 528, 20, console.window,
                                      (HMENU) INPUT_ID,
                                      window_vars.instance, NULL);

    /* create the buttons */
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


    /* create the scrollbuffer */
    console.window_buffer = CreateWindow("edit", NULL, CONSOLE_STYLE,
                                         6, 40, 526, 354, console.window,
                                         (HMENU) EDIT_ID,
                                         window_vars.instance, NULL);
    SendMessage(console.window_buffer, WM_SETFONT, (WPARAM) console.buffer_font, 0);

    console.input_line_handler = (WNDPROC) SetWindowLongPtr(console.input_line,
                                                            GWLP_WNDPROC,
                                                            (intptr_t) con_input_handler);
    SendMessage(console.input_line, WM_SETFONT, (WPARAM) console.buffer_font, 0);

    ShowWindow(console.window, SW_SHOWDEFAULT);
    UpdateWindow(console.window);
    SetForegroundWindow(console.window);
    SetFocus(console.input_line);

    console.vis_level = 1;
}

void con_destroy(void)
{
    if (console.window != NULL) {
        ShowWindow(console.window, SW_HIDE);
        CloseWindow(console.window);
        DestroyWindow(console.window);

        console.window = NULL;
    }
}
#else
void con_create(void)
{

}

void con_destroy(void)
{

}
#endif
