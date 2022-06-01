#ifndef IC_SHARED_H
#define IC_SHARED_H

#include <stdarg.h>

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
 * @brief Take a given path and append an extension (if one doesn't already
 * exist).
 *
 * If the given path string already has an extension (regardless if it is
 * the same as `ext`, will not append anything).
 *
 * @param[in,out] path string path
 * @param[in]     size max size of string
 * @param[in]     ext  extension to append
 */
IC_PUBLIC
IC_NON_NULL(1, 3)
void ic_default_extension(char *path, size_t size, const char *ext);

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
