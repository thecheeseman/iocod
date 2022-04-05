#include "shared.h"
#include "common.h"

#include "common/cbuf.h"
#include "common/error.h"
#include "common/hunk.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "system/shared.h"
#include "strings/stringlib.h"
#include "parse.h"
#include "va.h"

jmp_buf abortframe;

struct error_data {
    bool entered;
    int code;
    int lasterrtime;
    int count;
    char message[MAX_PRINT_MSG];
};

static struct error_data error = {
    .entered = false,
    .code = 0,
    .lasterrtime = 0,
    .count = 0,
    .message = '\0'
};

extern struct cvar *com_developer;
extern struct cvar *tty_colors;

bool com_error_entered(void)
{
    return error.entered;
}

static void INCOMPLETE com_error_internal(enum error_code code)
{
    int currenttime;

    // drop all to disconnect
    if (code == ERR_6 || code == ERR_7)
        code = ERR_DROP;

    error.entered = true;

    if (code != ERR_DISCONNECT &&
        code != ERR_NEED_CD &&
        code != ERR_END_OF_GAME) {
        cvar_get("com_errormessage", "", CVAR_ROM);
        cvar_set("com_errormessage", error.message);
    }

    hunk_swap_temp_low();
    hunk_swap_temp();

    if (code != ERR_DISCONNECT)
        scr_abort();

    /*
    bVar3 = DAT_0833df04 == -1;
    DAT_0833df04 = DAT_0833df04 + 1;
    if (bVar3) {
        DAT_0833df04 = 1;
    }
    _DAT_080e30cc = 0;
    */

    com_reset_parse_session();

    fs_clear_loadstack();

    if (code == ERR_DROP)
        cbuf_init();

    // fun_0806dedc(1, 0);
    // fun_0806dedc(2, 0);

    fs_pure_server_set_loaded_paks("", "");

    currenttime = sys_milliseconds();
    if (currenttime - error.lasterrtime < 100) {
        if (++error.count > 3)
            code = ERR_FATAL;
    } else {
        error.count = 0;
    }
    error.lasterrtime = currenttime;

    if (code == ERR_FATAL) {
        cl_shutdown();
        sv_shutdown(va("Server fatal crashed: %s", error.message));
        sv_remove_operator_commands();
        hunk_clear();
        com_shutdown();
        sys_error("%s", error.message);
        return;
    }

    error.code = code;
    longjmp(abortframe, -1);
}

void com_error_runner(enum error_code code, const char *file, const char *func,
                      int line, const char *fmt, ...)
{
    va_list argptr;
    char err[MAX_PRINT_MSG - 6];

    if (error.entered)
        sys_error("recursive error after: %s", error.message);

    va_start(argptr, fmt);
    vsnprintf(err, sizeof(err), fmt, argptr);
    va_end(argptr);

    snprintf(error.message, sizeof(error.message), "%s(%s:%d): %s",
             func, file, line, err);

    com_error_internal(code);
}

void com_error_msg_only(enum error_code code, const char *fmt, ...)
{
    va_list argptr;
    char err[MAX_PRINT_MSG - 6];

    if (error.entered)
        sys_error("recursive error after: %s", error.message);

    va_start(argptr, fmt);
    vsnprintf(err, sizeof(err), fmt, argptr);
    va_end(argptr);

    snprintf(error.message, sizeof(error.message), "%s", err);

    com_error_internal(code);
}

void INCOMPLETE com_error_handler(void)
{
    struct cvar *v;

    switch (error.code) {
    case ERR_SERVERDISCONNECT:
        sv_shutdown("Disconnected from own listen server");

        hunk_clear();
        cl_start_hunk_users();

        error.entered = false;
        break;
    case ERR_DROP:
    case ERR_DISCONNECT:
        if (tty_colors != NULL && tty_colors->integer > 0) {
            com_printf("\n\033[31m********************\nERROR: \033[0m%s\n\033[31m********************\033[0m\n\n",
                       error.message);
        } else {
            com_printf("\n********************\nERROR: %s\n********************\n\n",
                       error.message);
        }

        sv_shutdown(error.message);

        hunk_clear();
        cl_start_hunk_users();

        error.entered = false;

        if (error.code == ERR_DROP) {
            // not sure we need to implement this?
            v = cvar_get("r_vc_compile", "0", 0);
            //if (v->integer == 2)
            //    fun_0806d910();

            (void) v; // -Werror=unused-parameter
        }
        break;
    case ERR_NEED_CD:
        sv_shutdown("Server didn't have CD");

        hunk_clear();
        cl_start_hunk_users();

        if (com_cl_running != NULL && com_cl_running->integer > 0) {
            error.entered = false;
            cl_cd_dialog();
        } else {
            com_printf("Server didn't have CD\n");
        }

        break;
    case ERR_END_OF_GAME:
        sv_shutdown("End of game");

        hunk_clear();
        cl_start_hunk_users();

        error.entered = false;
        break;
    }

    // fun_0806cdca0(); // script stuff
}

void com_warning_runner(const char *file, const char *func, int line,
                        const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG - 6], dbgmsg[MAX_PRINT_MSG];

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    msg[strlen(msg)] = '\0';
    va_end(argptr);

    snprintf(dbgmsg, sizeof(dbgmsg), "%s(%s:%d): %s", func, file, line, msg);

    sys_warn(dbgmsg);
}

void com_debug_warning_runner(const char *file, const char *func, int line,
                              const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG - 6], dbgmsg[MAX_PRINT_MSG];

    if (com_developer == NULL || com_developer->integer == 0)
        return;

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    msg[strlen(msg)] = '\0';
    va_end(argptr);

    snprintf(dbgmsg, sizeof(dbgmsg), "%s(%s:%d): %s", func, file, line, msg);

    sys_warn(dbgmsg);
}
