#ifndef IC_MODULES_H
#define IC_MODULES_H

/*
 * MODULES
 * 
 * The module system begins by scanning the current directory and the 
 * `./modules` directory for any shared objects. It checks if each shared
 * object:
 *      1. can be loaded (i.e. proper binary)
 *      2. contains an exported `m_magic` symbol with correct magic number
 *      3. contains M_INFO_XXX() macros at the start of the file to store
 *         name/description of the module, version info, author name/email
 *      4. contains a `module_main` function with this exact prototype:
 *          @code
 *          M_EXPORT
 *          enum m_error module_main(struct m_module *m)
 *          @endcode
 *      5. contains a `module_free` function with this exact prototype:
 *          @code
 *          M_EXPORT
 *          void module_free(void)
 *          @endcode
 * 
 * If these four steps succeed, the module is then loaded and the system
 * will check if the module API version is different than the system API 
 * version. If it is, it will ignore the module. Optionally this functionality
 * can be overwritten (TODO: config value).
 * 
 * Then the `module_main` function is called. The system passes a 
 * preallocated empty `struct m_module *` to the `module_main` function, which
 * then the module may set up whatever it needs. If any failures occur,
 * modules should return an `MERR_` enum value to let the system know to
 * unload it.
 * 
 * MODULE FILES
 * 
 * The main module source file (which includes the functions `module_main` and 
 * `module_free`) should have the following at the top of the file:
 * 
 * @code
 * #include "ic_modules.h"
 * 
 * M_INFO_START()
 * M_INFO_NAME("name of module")
 * M_INFO_DESCRIPTION("short description of what the module does")
 * M_INFO_AUTHOR("your name here")
 * M_INFO_EMAIL("your email address ")
 * M_INFO_VERSION(1, 0, 0) // major, minor, patch version
 * M_INFO_END()
 * @endcode
 * 
 * @note `M_INFO_DESCRIPTION()` is optional
 * @warning All other macros are *mandatory*. Failure to include these in your
 * source file will result in the module _not_ being loaded.
 */

#include <stddef.h>

#define MODULE_DEBUG

#ifdef _WIN32
#define M_EXPORT __declspec(dllexport)
__pragma(warning(disable:4047)) /* differs in levels of indirection */
#else
#define M_EXPORT extern
#endif

/* stringify */
#define STRINGIFY_EX(x)      #x
#define STRINGIFY(x)         STRINGIFY_EX(x)

#define M_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))
#define M_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)
#define M_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)
#define M_VERSION_DECODE_PATCH(version)        ((version) % 1000)

#define M_API_VERSION_MAJOR 0
#define M_API_VERSION_MINOR 9
#define M_API_VERSION_PATCH 0
#define M_API_VERSION M_VERSION_ENCODE(M_API_VERSION_MAJOR, \
        M_API_VERSION_MINOR, M_API_VERSION_PATCH)

#define M_API_VERSION_STRING \
    STRINGIFY(M_API_VERSION_MAJOR) \
    "." STRINGIFY(M_API_VERSION_MINOR) \
    "." STRINGIFY(M_API_VERSION_PATCH)

#define M_INFO_START() \
    M_EXPORT const int _m_magic = 0x21730; \
    M_EXPORT const int _m_api_version = M_API_VERSION;
#define M_INFO_NAME(n) \
    M_EXPORT const char *_m_info_name = n;
#define M_INFO_DESCRIPTION(d) \
    M_EXPORT const char *_m_info_description = d;
#define M_INFO_AUTHOR(a) \
    M_EXPORT const char *_m_info_author = a;
#define M_INFO_EMAIL(e) \
    M_EXPORT const char *_m_info_email = e;
#define M_INFO_VERSION(maj, min, patch) \
    M_EXPORT const int _m_info_version = M_VERSION_ENCODE(maj, min, patch);
#define M_INFO_END()

enum m_error {
    MERR_OK,
    MERR_ALREADY_INIT,
    MERR_FAILED_MALLOC,
    MERR_NEW_VERSION
};

struct m_callback {
    const char *name;
    void (*func)(struct m_module *m, struct m_callback *c);
};

struct m_module {
    /* user modifiable fields */
    const char *long_name;      /// name of module
    const char *description;    /// short description
    const char *author;         /// author name
    const char *email;          /// author email
    int version;                /// module version

    /* internal fields that shouldn't be modified by users */
    size_t id;                  /// internal id that matches dynamic array
    const char *short_name;     /// always equal to filename
    int api_version;            /// api version module was built with

    char path[256];             /// exact path to module
    void *handle;               /// handle from dlsym

    enum m_error (*main)(struct m_module *);    /// func ptr to `module_main`
    void (*free)(void);                         /// func ptr to `module_free`

    int num_callbacks;              /// number of registered callbacks
    struct m_callback **callbacks;  /// list of callbacks
    enum m_error (*add_callback)(struct m_module *,
                                 const char *, void (*)(struct m_module *m,
                                                        struct m_callback *c));
};

enum message_hook_type {
    MHOOK_PRINTF,
    MHOOK_WARNING,
    MHOOK_ERROR,
    MHOOK_ERROR_FATAL
};

M_EXPORT
void m_message_hook(enum message_hook_type type, const char *fmt, ...);

#define m_printf(...)       m_message_hook(MHOOK_PRINTF, __VA_ARGS__)
#define m_warning(...)      m_message_hook(MHOOK_WARNING, __VA_ARGS__)
#define m_error(...)        m_message_hook(MHOOK_ERROR, __VA_ARGS__)
#define m_error_fatal(...)  m_message_hook(MHOOK_ERROR_FATAL, __VA_ARGS__)

#ifdef MODULE_DEBUG
#define m_debug_printf(...) m_printf(__VA_ARGS__)
#define m_debug_warning(...) m_warning(__VA_ARGS__)
#else
#define m_debug_printf(...)
#define m_debug_warning(...)
#endif

#endif /* IC_MODULES_H */
