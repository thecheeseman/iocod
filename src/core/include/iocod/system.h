#ifndef IC_SYSTEM_H
#define IC_SYSTEM_H

/**
 * @brief Get last library error, if applicable. Clears last error.
 * @return NULL-terminated string containing error message, otherwise NULL
*/
IC_PUBLIC
char *sys_library_error(void);

/**
 * @brief Load a system library.
 * @param[in]  path   file path to library module
 * @param[out] handle handle to loaded library module, NULL if failed
 * @return true if success, false otherwise
*/
IC_PUBLIC
bool sys_library_load(const char *path, void **handle);

/**
 * @brief Close a system library.
 * @param[in] handle loaded library module
 * @return true if success, false otherwise
*/
IC_PUBLIC
bool sys_library_close(void *handle);

/**
 * @brief Load a given @p symbol in a library module @p handle
 *
 * @note MSVC will throw warnings if you do not cast @p symbol to (void *)
 *
 * @param[in] handle  loaded library module
 * @param[in] fn      name of symbol to load
 * @param[out] symbol handle to symbol, NULL if failed
 * @return true if success, false otherwise
*/
IC_PUBLIC
bool sys_library_load_symbol(void *handle, const char *fn, void **symbol);

#endif /* IC_SYSTEM_H */
