#include "con_local.h"
#include <stdlib.h>

struct console_data condata;

#ifdef IC_PLATFORM_WINDOWS

#else
static void con_sigcont(int signum)
{
    con_init();
}

static bool stdin_is_atty(void)
{
    bool atty = isatty(STDIN_FILENO);

    const char *term = getenv("TERM");
    bool dumbterm = false;

    if (term == NULL || strcmp(term, "raw") == 0 || strcmp(term, "dumb") == 0)
        dumbterm = true;

    return atty && !dumbterm;
}

IC_PUBLIC
void con_init(void)
{
    memset(&condata, 0, sizeof(condata));

    // ignore tty in/tty out signals
    // if the process is running non-interactively these can turn into
    // a SIGSTP
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // if SIGCONT is received, reinit console
    signal(SIGCONT, con_sigcont);

    // make stdin non-blocking
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    if (!stdin_is_atty()) {
        ic_printf(_("tty console mode disabled\n"));

        condata.stdin_active = true;
        return;
    }

    memset(&condata.con, 0, sizeof(condata.con));

    tcgetattr(STDIN_FILENO, &condata.tc);
    condata.erase = condata.tc.c_cc[VERASE];
    condata.eof = condata.tc.c_cc[VEOF];

    struct termios tc = condata.tc;

    /*
        ECHO: don't echo input characters
        ICANON: enable canonical mode.  This  enables  the  special
        characters  EOF,  EOL,  EOL2, ERASE, KILL, REPRINT,
        STATUS, and WERASE, and buffers by lines.
        ISIG: when any of the characters  INTR,  QUIT,  SUSP,  or
        DSUSP are received, generate the corresponding signal
    */
    tc.c_lflag &= ~(ECHO | ICANON);

    /*
        ISTRIP strip off bit 8
        INPCK enable input parity checking
    */
    tc.c_iflag &= ~(ISTRIP | INPCK);

    tc.c_cc[VMIN] = 1;
    tc.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSADRAIN, &tc);

    condata.on = true;
    condata.hide = 1;

    con_show();
}

IC_PUBLIC
void con_shutdown(void)
{
    if (condata.on) {
        con_hide();
        tcsetattr(STDIN_FILENO, TCSADRAIN, &condata.tc);
    }

    // restore blocking to stdin reads
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) & ~O_NONBLOCK);
}
#endif
