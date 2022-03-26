#include "shared.h"
#include "common.h"

#include "commands/cbuf.h"
#include "common/error.h"
#include "common/hunk.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "parse.h"
#include "stringlib.h"

bool com_error_entered;
static char com_error_message[MAX_PRINT_MSG];
static int com_error_code;

jmp_buf abortframe;

extern struct cvar *com_developer;

/**
 * @brief
 * @param
 * @param file
 * @param func
 * @param line
 * @param fmt
 * @param
*/
void INCOMPLETE com_error_runner(enum error_code code, const char *file, const char *func,
                                 int line, const char *fmt, ...)
{
    va_list argptr;
    static int lasterrortime, errorcount;
    int currenttime;
    char err[MAX_PRINT_MSG - 6];

    if (com_error_entered)
        sys_error("recursive error after: %s", com_error_message);

    va_start(argptr, fmt);
    vsnprintf(err, sizeof(err), fmt, argptr);
    va_end(argptr);

    snprintf(com_error_message, sizeof(com_error_message), "%s(%s:%d): %s",
             func, file, line, err);

         // drop all to disconnect
    if (code == ERR_6 || code == ERR_7)
        code = ERR_DROP;

    com_error_entered = true;

    if (code != ERR_DISCONNECT &&
        code != ERR_NEED_CD &&
        code != ERR_END_OF_GAME) {
        cvar_get("com_errormessage", "", CVAR_ROM);
        cvar_set("com_errormessage", com_error_message);
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
    if (currenttime - lasterrortime < 100) {
        if (++errorcount > 3)
            code = ERR_FATAL;
    } else {
        errorcount = 0;
    }
    lasterrortime = currenttime;

    if (code == ERR_FATAL) {
        cl_shutdown();
        sv_shutdown(va("Server fatal crashed: %s", com_error_message));
        sv_remove_operator_commands();
        hunk_clear();
        com_shutdown();
        sys_error("%s", com_error_message);
        return;
    }

    com_error_code = code;
    longjmp(abortframe, -1);
}

void INCOMPLETE com_error_handler(void)
{
    struct cvar *v;

    switch (com_error_code) {
    case ERR_SERVERDISCONNECT:
        sv_shutdown("Disconnected from own listen server");

        hunk_clear();
        cl_start_hunk_users();

        com_error_entered = false;
        break;
    case ERR_DROP:
    case ERR_DISCONNECT:
        if (tty_colors != NULL && tty_colors->integer > 0) {
            com_printf("\n\033[31m********************\nERROR: \033[0m%s\n\033[31m********************\033[0m\n\n",
                       com_error_message);
        } else {
            com_printf("\n********************\nERROR: %s\n********************\n\n",
                       com_error_message);
        }

        sv_shutdown(com_error_message);

        hunk_clear();
        cl_start_hunk_users();

        com_error_entered = false;

        if (com_error_code == ERR_DROP) {
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
            com_error_entered = false;
            cl_cd_dialog();
        } else {
            com_printf("Server didn't have CD\n");
        }

        break;
    case ERR_END_OF_GAME:
        sv_shutdown("End of game");

        hunk_clear();
        cl_start_hunk_users();

        com_error_entered = false;
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
