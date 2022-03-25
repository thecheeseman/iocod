#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H

#include <setjmp.h>
#include <stdarg.h>

#include "types/bool.h"
extern bool com_error_entered;

enum error_code {
    ERR_FATAL,                  // exit the entire game with a popup window
    ERR_DROP,                   // print to console and disconnect from game
    ERR_SERVERDISCONNECT,       // don't kill server
    ERR_DISCONNECT,             // client disconnected from the server
    ERR_NEED_CD,                // pop up the need-cd dialog
    ERR_END_OF_GAME,
    ERR_6,
    ERR_7
};

/**
 * @brief Wrapper for com_error() - allow debug info to be printed
 * with src file/line info
 * @param code Error code
 * @param file Filename filled from __BASE_FILE__ macro
 * @param func Function name filled from __func__
 * @param line Line number filled from __LINE__
 * @param fmt Format of string
 * @param ... varargs
*/
void com_error_runner(enum error_code code, const char *file, const char *func,
                      int line, const char *fmt, ...);

void com_warning_runner(const char *file, const char *func, int line,
                        const char *fmt, ...);
void com_debug_warning_runner(const char *file, const char *func, int line,
                              const char *fmt, ...);

/**
 * @brief Raise an error
 * @param code Error code
 * @param args... Error message
*/
#define com_error(code, ...) \
	com_error_runner(code, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * @brief Wrapper for sys_warn for better warning messages
 * @param fmt Format of string
 * @param ... varargs
*/
#define com_warning(...) \
	com_warning_runner(__BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * @brief Wrapper for sys_warn for better warning messages
 * @param fmt Format of string
 * @param ... varargs
*/
#define com_debug_warning(...) \
	com_debug_warning_runner(__BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

extern jmp_buf abortframe;
void com_error_handler(void);

#endif /* COMMON_ERROR_H */
