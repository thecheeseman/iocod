#ifndef SYSTEM_WINDOWS_LOCAL_H
#define SYSTEM_WINDOWS_LOCAL_H

#include <windows.h>
#include "types/bool.h"

struct window_vars {
    HINSTANCE reflib_library;
    bool reflib_active;

    HWND window;
    HINSTANCE instance;
    bool active_app;
    bool is_minimized;
    OSVERSIONINFO os_version;

    unsigned int msg_time;
};

extern struct window_vars window_vars;

#endif /* SYSTEM_WINDOWS_LOCAL_H */
