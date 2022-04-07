#include "shared.h"
#include "common.h"

#include "common/error.h"
#include "cvar/cvar.h"
#include "strings/stringlib.h"
#include "system/windows/local.h"

void * DECL sys_load_dll(const char *name, char *fqpath,
                         intptr_t (DECL **entrypoint)(intptr_t, ...),
                         intptr_t (DECL *systemcalls)(intptr_t, ...))
{
    static int lastwarning = 0;
    HINSTANCE handle;
    void (DECL * dllentry)(intptr_t(DECL * syscallptr)(intptr_t, ...));
    char *basepath;
    char *cdpath;
    char *gamedir;
    char fn[MAX_OSPATH];
    char filename[MAX_OSPATH];

    *fqpath = '\0';

    snprintf(filename, sizeof(filename), "%s_mp_%s.%s", name, PLATFORM_ARCH,
             PLATFORM_DLL);

    basepath = cvar_variable_string("fs_basepath");
    cdpath = cvar_variable_string("fs_cdpath");
    gamedir = cvar_variable_string("fs_game");

    fs_build_ospath(basepath, gamedir, filename, fn);

    handle = LoadLibrary(fn);
    if (handle == NULL) {
        fs_build_ospath(basepath, "main", filename, fn); // probably don't need this
        handle = LoadLibrary(fn);

        if (handle == NULL) {
            handle = LoadLibrary(filename); // try current dir

            if (handle == NULL)
                return NULL;

            strncpyz(fqpath, filename, MAX_QPATH);
            goto skip_strncpyz; // ew
        }
    }

    strncpyz(fqpath, fn, MAX_QPATH);
skip_strncpyz:

    dllentry = (void (DECL *)(intptr_t(DECL *)(intptr_t, ...)))
             GetProcAddress(handle, "dllEntry");
    *entrypoint = (intptr_t(DECL *)(intptr_t, ...)) 
        GetProcAddress(handle, "vmMain");

    if (*entrypoint == NULL || dllentry == NULL) {
        FreeLibrary(handle);
        return NULL;
    }

    dllentry(systemcalls);
    return handle;
}

void sys_unload_dll(void *handle)
{
    if (handle == NULL)
        return;

    if (!FreeLibrary(handle))
        com_error(ERR_FATAL, "FreeLibrary failed");
}
