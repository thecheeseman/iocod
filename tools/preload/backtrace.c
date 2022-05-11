#include "preload.h"

struct termios *_termios = (struct termios *) 0x8318340;

/**
 * @brief use addr2line to grab data location
*/
static int addr2line(void const *const addr, const char *loc)
{
    char addr2line_cmd[512] = {0};

    sprintf(addr2line_cmd, "addr2line -C -i -f -p %p -e %s", addr, loc);
    return system(addr2line_cmd);
}

static void backtrace_stacktrace(void)
{
    int i, size;
    char *loc;
    char **msgs = (char **) NULL;
    void *traces[64];
    time_t t;
    struct tm *tm_info;
    char tmbuf[64];
    FILE *f;

    time(&t);
    tm_info = localtime(&t);
    strftime(tmbuf, 64, "%F %T", tm_info);

    f = fopen("backtrace.log", "a+");
    if (f)
        fprintf(f, "[%s] ********** STACKTRACE **********\n", tmbuf);

    size = backtrace(traces, 64);

    fprintf(stderr, "backtrace() returned %d addresses\n", size);
    if (f)
        fprintf(f, "[%s] backtrace() returned %d addresses\n", tmbuf, size);

    msgs = backtrace_symbols(traces, size);
    if (msgs != NULL) {
        for (i = 0; i < size; ++i) {
            fprintf(stderr, "%s ", msgs[i]);

            if (f)
                fprintf(f, "[%s] %s\n", tmbuf, msgs[i]);

            loc = strtok(msgs[i], "(");
            addr2line(traces[i], loc);
        }

        free(msgs);
        fflush(stderr);

        if (f) {
            fprintf(f, "[%s] ********** STACKTRACE **********\n", tmbuf);
            fclose(f);
        }
    }
}

/**
 * Handle any singals received.
 */
static void backtrace_handler(int sig, siginfo_t *siginfo, void *context)
{
    char *error;
    (void) context; // -Werror=unused-parameter

    tcsetattr(0, 1, _termios);

    switch (sig) {
        case SIGSEGV:                   error = "SIGSEGV: segmentation fault"; break;
        case SIGINT:                    error = "SIGINT: interactive attention signal"; break;
        case SIGFPE:
            switch (siginfo->si_code) {
                case FPE_INTDIV:        error = "SIGFPE: integer divide by zero"; break;
                case FPE_INTOVF:        error = "SIGFPE: integer overflow"; break;
                case FPE_FLTDIV:        error = "SIGFPE: floating-point divide by zero"; break;
                case FPE_FLTOVF:        error = "SIGFPE: floating-point overflow"; break;
                case FPE_FLTUND:        error = "SIGFPE: floating-point underflow"; break;
                case FPE_FLTRES:        error = "SIGFPE: floating-point inexact result"; break;
                case FPE_FLTINV:        error = "SIGFPE: floating-point invalid operation"; break;
                case FPE_FLTSUB:        error = "SIGFPE: subscript out of range"; break;
                default:                error = "SIGFPE: arithmetic exception"; break;
            }
            break;
        case SIGILL:
            switch (siginfo->si_code) {
                case ILL_ILLOPC:    error = "SIGILL: illegal opcode"; break;
                case ILL_ILLOPN:    error = "SIGILL: illegal operand"; break;
                case ILL_ILLADR:    error = "SIGILL: illegal addressing mode"; break;
                case ILL_ILLTRP:    error = "SIGILL: illegal trap"; break;
                case ILL_PRVOPC:    error = "SIGILL: privileged opcode"; break;
                case ILL_PRVREG:    error = "SIGILL: privileged register"; break;
                case ILL_COPROC:    error = "SIGILL: coprocessor error"; break;
                case ILL_BADSTK:    error = "SIGILL: internal stack error"; break;
                default:            error = "SIGILL: illegal instruction"; break;
            }
            break;
        case SIGTERM:                   error = "SIGTERM: a termination request was sent to the program"; break;
        case SIGABRT:                   error = "SIGABRT: usually caused by abort() or assert()"; break;
        default:                        error = "SIGUNKN: unknown signal"; break;
    }

    fprintf(stderr, "****************************************\n");
    fprintf(stderr, "ERROR: %s\n", error);
    fprintf(stderr, "****************************************\n");
    fflush(stderr);

    backtrace_stacktrace();
    exit(0);
}

/**
 * @brief Catch interrupt signals
*/
static void backtrace_interrupt_handler(int signal)
{
    static bool sigint_caught = false;
    if (sigint_caught) {
        tcsetattr(0, 1, _termios);
        exit(0);
    }
    sigint_caught = true;

    printf("\nPress Ctrl-C twice to terminate\n");
    fflush(stdout);
}

/**
 * @brief
*/
void backtrace_init(enum backtrace_options options)
{
    uint8_t altstack[SIGSTKSZ];
    {
        stack_t ss;
        ss.ss_sp = (void *) altstack;
        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;

        if (sigaltstack(&ss, NULL) != 0)
            err(1, "sigaltstack");
    }

    {
        struct sigaction sig_action;
        sig_action.sa_sigaction = backtrace_handler;
        sigemptyset(&sig_action.sa_mask);

        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;

        if (options & BT_SEGV && sigaction(SIGSEGV, &sig_action, NULL) != 0)
            err(1, "sigaction");
        if (options & BT_FPE && sigaction(SIGFPE, &sig_action, NULL) != 0)
            err(1, "sigaction");
        if (options & BT_INT && sigaction(SIGINT, &sig_action, NULL) != 0)
            err(1, "sigaction");
        if (options & BT_ILL && sigaction(SIGILL, &sig_action, NULL) != 0)
            err(1, "sigaction");
        if (options & BT_TERM && sigaction(SIGTERM, &sig_action, NULL) != 0)
            err(1, "sigaction");
        if (options & BT_ABRT && sigaction(SIGABRT, &sig_action, NULL) != 0)
            err(1, "sigaction");
    }

    // override for SIGINT to catch ^C
    signal(SIGINT, backtrace_interrupt_handler);
}
