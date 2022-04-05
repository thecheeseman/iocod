#include <time.h>

#include "shared.h"
#include "common.h"

#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"
#include "system/shared.h"
#include "cvar/cvar.h"
#include "stringlib.h"

static char *rd_buffer;
static int rd_buffersize;
static void (*rd_flush)(char *buffer);

extern struct cvar *com_dedicated;
extern struct cvar *com_logfile;
extern struct cvar *tty_colors;

FILE *debuglogfile;
filehandle logfile;
filehandle com_journalfile;
filehandle com_journaldatafile;

static bool opening_qconsole;

void com_begin_redirect(char *buffer, int buffersize, void (*flush)(char *))
{
    if (buffer == NULL || buffersize == 0 || flush == NULL)
        return;

    rd_buffer = buffer;
    rd_buffersize = buffersize;
    rd_flush = flush;

    *rd_buffer = 0;
}

void com_end_redirect(void)
{
    if (rd_flush != NULL)
        rd_flush(rd_buffer);

    rd_buffer = NULL;
    rd_buffersize = 0;
    rd_flush = NULL;
}

void com_print_header(const char *text, size_t size, char sep)
{
    char *header;
    size_t i, j, len, lpad, rpad;

    if (size < strlen(text))
        com_error(ERR_FATAL, "tried to print header where size < strlen(text)");

    header = z_malloc(size + 1);
    for (i = 0; i < (size + 1); i++)
        header[i] = sep;
    header[size] = '\0';

    len = strlen(text);
    lpad = (size / 2) - (len / 2);
    rpad = (size / 2) + (len / 2);

    if (rpad > size)
        rpad = size;

    // if we have a string, otherwise just print the separator 
    if (len) {
        header[lpad - 1] = ' ';

        for (i = lpad, j = 0; i < (len + lpad); i++, j++)
            header[i] = text[j];

        header[i] = ' ';
    }

    com_printf("%s\n", header);
    z_free(header);
}

void com_printf_runner(enum print_level level, const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG];

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    msg[strlen(msg)] = '\0';
    va_end(argptr);

    // is this even really used??
    if (rd_buffer != NULL) {
        if (level != PRINT_LOGONLY) {
            if ((strlen(msg) + strlen(rd_buffer)) >
                (unsigned) (rd_buffersize - 1)) {
                rd_flush(rd_buffer);
                *rd_buffer = 0;
            }

            strncatz(rd_buffer, rd_buffersize, msg);
        }

        return;
    }

    // level == PRINT_LOGONLY seems to be only used for cvar_set2 
    // to hide "cvar set %s %s" message in console
    if (level != PRINT_LOGONLY) {
        // echo to console if we're not a dedicated server
        if (com_dedicated != NULL && com_dedicated->integer == 0)
            cl_console_print(msg);

        // echo to dedicated console
        sys_print(msg);
    }

    // logfile stuff
    if (com_logfile != NULL && com_logfile->integer > 0) {
        if (logfile == 0 && fs_initialized() && !opening_qconsole) {
            struct tm *newtime;
            time_t aclock;

            opening_qconsole = true;
            time(&aclock);
            newtime = localtime(&aclock);

            logfile = fs_fopen_file_write("console_mp_server.log");
            com_printf("logfile opened on %s\n", asctime(newtime));

            // force it to not buffer
            if (com_logfile->integer > 1)
                fs_force_flush(logfile);

            opening_qconsole = false;
        }

        if (logfile > 0 && fs_initialized())
            fs_write(msg, strlen(msg), logfile);
    }
}

void com_printf_dbginfo_runner(const char *file, const char *func, int line,
                               const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG - 6], dbgmsg[MAX_PRINT_MSG];

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    msg[strlen(msg)] = '\0';
    va_end(argptr);

    snprintf(dbgmsg, sizeof(dbgmsg), "%s(%s:%d): %s", func, file, line, msg);

    com_printf(dbgmsg);
}

void com_dprintf(const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG];

    if (com_developer == NULL || com_developer->integer == 0)
        return;

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if (tty_colors && tty_colors->integer)
        com_printf("\033[36m%s\033[0m", msg);
    else
        com_printf("%s", msg);

    // TODO: developer logging?
}
