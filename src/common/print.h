#ifndef COMMON_PRINT_H
#define COMMON_PRINT_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "types/filehandle.h"

#define MAX_PRINT_MSG 4096

enum print_level {
    PRINT_ALL,
    PRINT_DEVELOPER,
    PRINT_WARNING,
    PRINT_ERROR,
    PRINT_LOGONLY
};

/**
 * @brief Print a nice header
*/
void com_print_header(const char *text, size_t size, char sep);

/**
 * @brief Wrapper for printf with ability to supress console text
 * @param code Can be used to force printf to logfiles only (PRINT_LOGONLY)
 * @param fmt Format of string message
 * @param ... Message to print
*/
void com_printf_runner(enum print_level level, const char *fmt, ...);

/**
 * @brief Wrapper for com_printf_dbginfo() - allow debug info to be printed
 * with src file/line info
 * @param file Filename filled from __BASE_FILE__ macro
 * @param func Function name filled from __func__
 * @param line Line number filled from __LINE__
 * @param fmt Format of string message
 * @param ... Message to print
*/
void com_printf_dbginfo_runner(const char *file, const char *func, int line,
                               const char *fmt, ...);

/**
 * @brief Debug printf that only prints when cvar `developer` >= 1
 * @param fmt Format of string message
 * @param ... Message to print
*/
void com_dprintf(const char *fmt, ...);

/**
 * @brief Wrapper for printf which defaults to PRINT_ALL to console
 * @param ... Message to print
*/
#define com_printf(...) com_printf_runner(PRINT_ALL, __VA_ARGS__)

/**
 * @brief Wrapper for printf which defaults to PRINT_LOGONLY to console
 * @param ... Message to print
*/
#define com_printf_logonly(...) com_printf_runner(PRINT_LOGONLY, __VA_ARGS__)

/**
 * @brief Allow debug info to be printed with src file/line info
 * @param args... Message to print
*/
#define com_printf_dbginfo(...) \
	com_printf_dbginfo_runner(__BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

extern FILE *debuglogfile;
extern filehandle logfile;
extern filehandle com_journalfile;
extern filehandle com_journaldatafile;

#endif /* COMMON_PRINT_H */
