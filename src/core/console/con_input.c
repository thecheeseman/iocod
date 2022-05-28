#include "console.h"

#ifdef IC_PLATFORM_WINDOWS
void con_input_handler(HWND window, UINT message, WPARAM param1, LPARAM param2)
{
    switch (message) {
    case WM_KILLFOCUS:
        if ((HWND) param1 == console.window ||
            (HWND) param1 == console.error_box) {
            SetFocus(window);
            return 0;
        }
        break;

    case WM_CHAR:
        if (param1 == 0x0D) { /* CR */
            char buf[1024];

            GetWindowText(console.input_line, buf, sizeof(buf));

            strncat(console.console_text, buf,
                    sizeof(console.console_text) - 
                    strlen(console.console_text) - 5);
            strcat(console.console_text, "\n");

            SetWindowText(console.input_line, "");

            con_print(va("]%s\n", buf));

            return 0;
        }
    }

    return CallWindowProc(console.input_line_handler, window, message,
                          param1, param2);
}

void con_input_init(void)
{

}

void con_input_shutdown(void)
{

}
#else
void con_input_init(void)
{
    console.ttycon = cv_get("ttycon", "1", CV_NONE);

    if (isatty(STDIN_FILENO) != 1) {
        fprintf(stderr, "stdin is not a tty, tty console mode failed\n");
        cv_set_integer("ttycon", 0);
        console.ttycon_enabled = false;
        return;
    }

    printf("started tty console (use +set ttycon 0 to disable)\n");

    field_clear(&console.ttycon);
    tcgetattr(0, &console.tty_tc);

    struct termios tc;
    console.tty_erase = tc.c_cc[VERASE];
    console.tty_eof = tc.c_cc[VEOF];
    tc = console.tty_tc;

    /*
    ECHO: don't echo input characters
    ICANON: enable canonical mode.  This  enables  the  special
            characters  EOF,  EOL,  EOL2, ERASE, KILL, REPRINT,
            STATUS, and WERASE, and buffers by lines.
    ISIG: when any of the characters  INTR,  QUIT,  SUSP,  or
            DSUSP are received, generate the corresponding sig­
            nal
    */
    tc.c_lflag &= ~(ECHO | ICANON);
    /*
     ISTRIP strip off bit 8
     INPCK enable input parity checking
     */
    tc.c_iflag &= ~(ISTRIP | INPCK);
    tc.c_cc[VMIN] = 1;
    tc.c_cc[VTIME] = 0;

    tcsetattr(0, TCSADRAIN, &tc);
    console.ttycon_enabled = true;
}

void con_input_shutdown(void)
{
    if (console.ttycon_enabled) {
        fprintf(stderr, "Shutdown ttyconsole\n");

        tcsetattr(0, TCSADRAIN, &console.tty_tc);
    }
}
#endif
