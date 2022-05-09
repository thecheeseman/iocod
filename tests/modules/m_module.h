#ifndef M_MODULE_H
#define M_MODULE_H

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
 * #include "m_module.h"
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

#define M_DEBUG

#ifdef _WIN32
#define M_EXPORT    __declspec(dllexport)
#define M_DECL      __cdecl

__pragma(warning(disable:4047)) /* differs in levels of indirection */
#else
#define M_EXPORT
#define M_DECL
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
#define M_API_VERSION_MINOR 20
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

/*
 * Module callbacks
 */
enum m_callback {
    M_INIT,                     /// module init stage
    M_FREE,                     /// module free stage
    M_TEST_CALLBACK,

    M_COM_INIT = 0x10,          /// called before `M_SCRIPT_INIT`
    M_SCRIPT_INIT,
    M_SV_INIT,
    M_COM_INIT_POST,            /// called at the end of `com_init()`

    M_COM_SHUTDOWN = 0x20,      /// called before `M_SCRIPT_SHUTDOWN`
    M_SCRIPT_SHUTDOWN,
    M_COM_SHUTDOWN_POST,        /// called at the end of `com_shutdown()`

    M_COM_FRAME = 0x30,         /// called before `M_SV_FRAME`
    M_SV_FRAME,
    M_CL_FRAME,
    M_COM_FRAME_POST,           /// called at the end of `com_frame()`

    M_MAX_CALLBACKS
};

struct m_info {
    const char *name;           /// name of module
    const char *description;    /// short description
    const char *author;         /// author name
    const char *email;          /// author email
    int version;                /// module version
    int api_version;            /// api version module is built for
};

#define M_SETUP_ARG_COUNT 16
/**
 * @def M_SETUP_ARGS
 * @brief Set up module variable arguments.
 *
 * This is necessary for module_main communication to work with the module
 * system.
 */
#define M_SETUP_ARGS(x) \
    m_ptr args[M_SETUP_ARG_COUNT]; \
    args[0] = (x); \
    va_list ap; \
    va_start(ap, (x)); \
    for (m_ptr i = 1; i < M_SETUP_ARG_COUNT; i++) \
        args[i] = va_arg(ap, m_ptr); \
    va_end(ap);

/*
 * Syscalls available to the module system
 */
enum m_syscall {
    M_PRINTF,           /// printf
    M_DPRINTF,          /// debug printf
    M_WARNING,          /// warning
    M_DWARNING,         /// debug warning
    M_ERROR,            /// error
    M_ERROR_FATAL,      /// fatal error

    M_CALLBACK_NAME,    /// return the name of the callback

    M_MAX_SYSCALLS
};

#ifndef M_INTERNAL
char *M_DECL va(const char *fmt, ...);
extern void m_printf(const char *fmt);
extern void m_dprintf(const char *fmt);
extern void m_warning(const char *fmt);
extern void m_dwarning(const char *fmt);
extern void m_error(const char *fmt);
extern void m_error_fatal(const char *fmt);
extern const char *m_callback_name(int callback);
#endif

#endif /* M_MODULE_H */
