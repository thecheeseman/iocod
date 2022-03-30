#include <windows.h>
#include "types/bool.h"

struct winvars {
    // not sure
    HINSTANCE reflib_library;
    bool reflib_active;

    HWND hWnd;
    HINSTANCE hInstance;
    bool activeApp;
    bool isMinimized;
    OSVERSIONINFO osVersion;

    unsigned int msg_time;
};

extern struct winvars g_wv;

void console_create(void);
void console_destroy(void);
void console_show(int visLevel, bool quitOnClose);
void console_append(const char *pMsg);
void set_error_text(const char *buf);
