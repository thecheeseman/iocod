#include <windows.h>
#include <VersionHelpers.h>

#include "shared.h"
#include "common.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "system/shared.h"
#include "system/windows/local.h"
#include "va.h"

void sys_exit(void)
{

}

#define OSR2_BUILD_NUMBER 1111
#define WIN98_BUILD_NUMBER 1998

void sys_init(void)
{
    int cpuid;
    const char *arch = "";
    OSVERSIONINFO *version = &window_vars.os_version;

    timeBeginPeriod(1);

    cmd_add_command("in_restart", in_restart_f);

    if (IsWindows10OrGreater()) {
        arch = "win10";
    } else if (IsWindows8OrGreater()) {
        arch = "win8";
    } else if (IsWindows7OrGreater()) {
        arch = "win7";
    } else if (IsWindowsVistaOrGreater()) {
        arch = "winvista";
    } else if (IsWindowsXPOrGreater()) {
        arch = "winxp";
    } else {
        // really old version ??
        version->dwOSVersionInfoSize = sizeof(version);

        if (!GetVersionEx(&version))
            sys_error("Couldn't get OS info");

        // can't imagine anyone is building for these but
        // they're here for compatibility
        if (version->dwPlatformId == VER_PLATFORM_WIN32_NT) {
            arch = "winnt";
        } else if (version->dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
            if (LOWORD(version->dwBuildNumber) >= WIN98_BUILD_NUMBER)
                arch = "win98";
            else if (LOWORD(version->dwBuildNumber) >= OSR2_BUILD_NUMBER)
                arch = "win95 osr2.x";
            else
                arch = "win95";
        } else {
            arch = "unknown Windows variant";
        }
    }

    cvar_set("arch", arch);

    cvar_get("win_hinstance", va("%i", (intptr_t) window_vars.instance), CVAR_ROM);
    //cvar_get("win_wndproc", va("%i", (intptr_t) main_window_handler), CVAR_ROM);

    cvar_get("sys_cpustring", PLATFORM_ARCH, 0);
    com_printf("%s\n", cvar_variable_string("sys_cpustring"));

    cvar_set("username", sys_get_current_user());

    in_init();
}

void sys_warn(char *warning, ...)
{

}
