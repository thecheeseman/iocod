#include "ic_modules.h"

static const char *callback_names[] = {
    "M_INIT",
    "M_FREE",

    [0x10] = "M_COM_INIT",
    "M_SCRIPT_INIT",
    "M_SV_INIT",
    "M_COM_INIT_POST",

    [0x20] = "M_COM_SHUTDOWN",
    "M_SCRIPT_SHUTDOWN",
    "M_COM_SHUTDOWN_POST",

    [0x30] = "M_COM_FRAME",
    "M_SV_FRAME",
    "M_CL_FRAME",
    "M_COM_FRAME_POST",

    "M_MAX_CALLBACKS"
};

const char *get_callback_name(int callback)
{
    if (callback < 0 || callback > M_MAX_CALLBACKS)
        return "<unknown>";

    return callback_names[callback];
}

/**
 * @brief Hook for modules to print things to the console
 *
 * @param type hook type
 * @param fmt format string
 * @param ...
 *
 * @note It is recommended to use the macros `m_printf()`, `m_warning()`,
 * `m_error()`, and `m_error_fatal()` instead of calling this directly
*/
void m_message_hook(enum m_syscall type, const char *fmt, ...)
{
    char msg[1024];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    switch (type) {
    case M_PRINTF:
        ic_printf("%s", msg);
        break;
    case M_DPRINTF:
        #ifdef M_DEBUG
        ic_printf("%s", msg);
        #endif
        break;
    case M_WARNING:
        ic_warning("%s", msg);
        break;
    case M_DWARNING:
        #ifdef M_DEBUG
        ic_warning("%s", msg);
        #endif
        break;
    case M_ERROR:
        ic_error("%s", msg);
        break;
    case M_ERROR_FATAL:
        ic_error_fatal("%s", msg);
        break;
    default:
        break;
    }
}

/**
 * @brief Syscall hook for all modules
 */
m_ptr M_DECL m_syscall(m_ptr command, ...)
{
    M_SETUP_ARGS(command);

    switch (command) {
    case M_PRINTF:
    case M_DPRINTF:
    case M_WARNING:
    case M_DWARNING:
    case M_ERROR:
    case M_ERROR_FATAL:
        m_message_hook(command, "%s", (char *) args[1]);
        break;
    case M_CALLBACK_NAME:
        return (m_ptr) get_callback_name(args[1]);
    default:
        m_debug_printf("unknown syscall %d\n", command);
    }

    return 0;
}
