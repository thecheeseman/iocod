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
#include <stdint.h>
#include <stdarg.h>

#define MODULE_DEBUG

#ifdef _WIN32
#define M_EXPORT __declspec(dllexport)
__pragma(warning(disable:4047)) /* differs in levels of indirection */
#else
#define M_EXPORT 
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
#define M_API_VERSION_MINOR 10
#define M_API_VERSION_PATCH 0
#define M_API_VERSION M_VERSION_ENCODE(M_API_VERSION_MAJOR, \
        M_API_VERSION_MINOR, M_API_VERSION_PATCH)

#define M_API_VERSION_STRING \
    STRINGIFY(M_API_VERSION_MAJOR) \
    "." STRINGIFY(M_API_VERSION_MINOR) \
    "." STRINGIFY(M_API_VERSION_PATCH)

typedef intptr_t m_ptr;

enum m_error {
    M_OK,
    M_ERR_ALREADY_INIT,
    M_ERR_FAILED_MALLOC,
    M_ERR_NEW_VERSION
};

/**
 * @def M_INFO_BEGIN
 * @brief Begin module information section.
 * 
 * This must be called after including the module header, and before
 * any code segment in your module's main source file. This information
 * is used to provide the module system with knowledge of what it's
 * actually loading, so please make sure to provide this.
 */
#define M_INFO_BEGIN() M_EXPORT struct m_module_info module_info = {

/**
 * @def M_INFO_NAME
 * @brief Name of your module
 * @param n name
 */
#define M_INFO_NAME(n)          .name = n,

/**
 * @def M_INFO_DESCRIPTION
 * @brief Brief description of what your module does.
 * @param d description
 */
#define M_INFO_DESCRIPTION(d)   .description = d,

/**
 * @def M_INFO_AUTHOR
 * @brief Your name or username.
 * @param a author name
 */
#define M_INFO_AUTHOR(a)        .author = a,

/**
 * @def M_INFO_EMAIL
 * @brief An email address you can be reached at.
 * @param e email address
 */
#define M_INFO_EMAIL(e)         .email = e,

/**
 * @def M_INFO_VERSION
 * @brief Your module's version information.
 * @param m major version
 * @param n minor version
 * @param p patch version
 */
#define M_INFO_VERSION(m, n, p) .version = M_VERSION_ENCODE(m, n, p),

/**
 * @def M_INFO_END
 * @brief End module information section.
 * 
 * This must be called after all module information has been set. It will
 * automatically append the current `M_API_VERSION` of the module API you
 * have installed, so other systems can know if they are compatible with
 * your module.
 */
#define M_INFO_END()            .api_version = M_API_VERSION };

#define M_SETUP_ARG_COUNT 4
/**
 * @def M_SETUP_ARGS
 * @brief Set up module variable arguments.
 * 
 * This is necessary for module_main communication to work with the module
 * system.
 */
#define M_SETUP_ARGS() \
    m_ptr args[M_SETUP_ARG_COUNT]; \
    va_list ap; \
    va_start(ap, command); \
    for (m_ptr i = 0; i < M_SETUP_ARG_COUNT; i++) \
        args[i] = va_arg(ap, m_ptr); \
    va_end(ap);

enum m_callbacks {
    M_INIT,                     /// module init stage
    M_FREE,                     /// module free stage
    M_TEST_CALLBACK             
};

struct m_module_info {
    /* user modifiable fields */
    const char *name;           /// name of module
    const char *description;    /// short description
    const char *author;         /// author name
    const char *email;          /// author email
    int version;                /// module version
    int api_version;            /// api version module is built for
};

struct m_module {
    struct m_module_info *info; /// module info passed from module

    size_t id;                  /// internal id that matches dynamic array
    const char *short_name;     /// always equal to filename

    char path[256];             /// exact path to module
    void *handle;               /// handle from dlsym

    m_ptr (*main)(m_ptr command, ...); /// func ptr to `module_main`
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
