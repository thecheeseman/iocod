#ifndef IC_SHARED_H
#define IC_SHARED_H

#include "iocod.h"

#include <stdarg.h>

enum q3color {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_WHITE
};

/**
 * @brief Get current milliseconds since application began.
 * @return 
*/
IC_PUBLIC
int32_t ic_milliseconds(void);

/**
 * @brief Formatted string concatenation.
 *
 * @param[in] fmt string format
 * @return formatted string
 */
IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
char *va(const char *fmt, ...);

#define MAX_INFO_STRING 1024
#define INFO_STRING_BIG 8192

/**
 * @brief Take a given path @p path and append extension @p ext if it does
 * not already exist.
 * 
 * This function is a replacement of Q3's `Com_DefaultExtension` since that
 * function only cares if the path _has_ an extension, not whether that
 * extension is correct.
 * 
 * @note @p ext is case-sensitive.
 * @note If extension already exists, but is different, this will append the
 * new extension to the end -- e.g. if @p ext is `".dll"`, and @p path is
 * `"test.ext"`, the new path will be `"test.ext.dll"`
 *
 * @param[in,out] path NULL-terminated path string
 * @param[in]     size max size of @p path
 * @param[in]     ext  extension to append
 * @return true if success (or nothing was done), false if invalid parameters
 * are specified
 */
IC_PUBLIC
bool ic_append_extension(char *path, size_t size, const char *ext);

/**
 * @brief Remove a key/value pair from an info string.
 *
 * @param[in,out] s   info string to modify
 * @param[in]     key key to remove
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_remove_key(char *s, const char *key);

/**
 * @brief Remove a key/value pair from a big info string.
 *
 * @param[in,out] s   info string to modify
 * @param[in]     key key to remove
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_remove_key(char *s, const char *key);

/**
 * @brief Change or add a key/value pair to an info string.
 *
 * @param[in,out] s     info string to modify
 * @param[in]     key   key to add
 * @param[in]     value value of key
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_set_value_for_key(char *s, const char *key, const char *value);

/**
 * @brief Change or add a key/value pair to a big info string.
 *
 * @param[in,out] s     info string to modify
 * @param[in]     key   key to add
 * @param[in]     value value of key
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_set_value_for_key(char *s, const char *key, const char *value);

IC_PUBLIC
/**
 * @brief Create temporary vectors.
 *
 * @param[in] x
 * @param[in] y
 * @param[in] z
 * @return temporary vector of (x, y, z)
*/
vec_t *tv(vec_t x, vec_t y, vec_t z);

#endif /* IC_SHARED_H */
