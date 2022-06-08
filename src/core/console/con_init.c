#include "con_local.h"

struct console_data console;

#ifdef IC_PLATFORM_WINDOWS
static BOOL WINAPI con_sigint(DWORD sig)
{
    // TODO:
    return TRUE;
}

IC_PUBLIC
void con_init(void)
{
    memset(&console, 0, sizeof(console));

    SetConsoleCtrlHandler(con_sigint, TRUE);

    console.hin = GetStdHandle(STD_INPUT_HANDLE);
    if (console.hin == INVALID_HANDLE_VALUE)
        return;

    console.hout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console.hout == INVALID_HANDLE_VALUE)
        return;

    // enable mouse wheel scrolling + virtual terminal sequences
    GetConsoleMode(console.hin, &console.original_mode);
    SetConsoleMode(console.hin, console.original_mode & 
                   (~ENABLE_MOUSE_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT));

    FlushConsoleInputBuffer(console.hin);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(console.hout, &info);
    console.attributes = info.wAttributes;
    console.bg_attributes = console.attributes & 
        (BACKGROUND_BLUE | BACKGROUND_GREEN | 
         BACKGROUND_RED | BACKGROUND_INTENSITY);

    con_set_title("iocod " IC_VERSION_STRING " dedicated server console");

    // init history tbd

    SetConsoleTextAttribute(console.hout, console.attributes);
}

IC_PUBLIC
void con_shutdown(void)
{
    con_hide();

    SetConsoleMode(console.hin, console.original_mode);
    SetConsoleCursorInfo(console.hout, &console.original_cursorinfo);
    SetConsoleTextAttribute(console.hout, console.attributes);

    CloseHandle(console.hout);
    CloseHandle(console.hin);
}
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
    memset(&console, 0, sizeof(console));

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

        console.stdin_active = true;
        return;
    }

    memset(&console.field, 0, sizeof(console.field));

    tcgetattr(STDIN_FILENO, &console.tc);
    console.tty_erase = console.tc.c_cc[VERASE];
    console.tty_eof = console.tc.c_cc[VEOF];

    struct termios tc = console.tc;

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

    console.on = true;
    console.hide = 1;

    con_set_title("iocod " IC_VERSION_STRING " dedicated server console");

    con_show();
}

IC_PUBLIC
void con_shutdown(void)
{
    if (console.on) {
        con_hide();
        tcsetattr(STDIN_FILENO, TCSADRAIN, &console.tc);
    }

    // restore blocking to stdin reads
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) & ~O_NONBLOCK);
}
#endif
