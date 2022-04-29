#ifndef IC_MODULES_H
#define IC_MODULES_H

/*
 * MODULES
 * 
 * The module system begins by scanning the current directory and the 
 * `./modules` directory for any shared objects. It checks if each shared
 * object:
 *      1. can be loaded (i.e. proper binary)
 *      2. contains a `module_api_version` function
 *      3. contains a `module_main` function with this exact prototype:
 *          `enum m_error module_main(struct m_module *m)`
 *      4. contains a `module_free` function with this exact prototype:
 *          `void module_free(void)`
 * 
 * @note The `module_api_version` function is automatically added by the 
 * system, users do not need to write one on their own.
 * 
 * If these four steps succeed, the module is then loaded and 
 * `module_api_version` is called. This version is checked to see if the
 * module's version is _newer_ than the API version, and if it is, it will
 * be ignored.
 * 
 * Then the `module_main` function is called. The system passes a 
 * preallocated empty `struct m_module *` to the `module_main` function, 
 * which then must be set up by the module.
 * 
 * MODULE_MAIN
 * Modules must do the following when `module_main` is called:
 *      1. set the following structure fields:
 *          @code
 *          m->long_name = "long name";
 *          m->author = "author name";
 *          m->email = "email@address.com";
 *          m->version = MODULE_VERSION_ENCODE(1, 0, 0);
 *          @endcode
 *      2. optionally the `m->description` field may be set
 *      3. set up any callbacks via `m->add_callback()`
 *      4. after setup is complete, return MERR_OK
 * 
 * If module does not provide any info, no callbacks will ever be made and
 * a call to `module_free` will immediately occur. 
 */

#include <stddef.h>

#define MODULE_DEBUG

#ifdef _WIN32
#define MODULE_EXPORT __declspec(dllexport)
#else
#define MODULE_EXPORT extern
#endif

/* stringify */
#define STRINGIFY_EX(x)      #x
#define STRINGIFY(x)         STRINGIFY_EX(x)

#define MODULE_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))
#define MODULE_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)
#define MODULE_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)
#define MODULE_VERSION_DECODE_PATCH(version)        ((version) % 1000)

#define MOD_API_VERSION_MAJOR 0
#define MOD_API_VERSION_MINOR 5
#define MOD_API_VERSION_PATCH 0
#define MOD_API_VERSION MODULE_VERSION_ENCODE(MOD_API_VERSION_MAJOR, \
        MOD_API_VERSION_MINOR, MOD_API_VERSION_PATCH)

#define MOD_API_VERSION_STRING \
    STRINGIFY(MOD_API_VERSION_MAJOR) \
    "." STRINGIFY(MOD_API_VERSION_MINOR) \
    "." STRINGIFY(MOD_API_VERSION_PATCH)


#ifndef MODULE_INTERNAL
/**
 * @brief Returns API version module was built with
*/
MODULE_EXPORT
int module_api_version(void)
{
    return MOD_API_VERSION;
}
#endif

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

    char path[256];             /// exact path to module
    void *handle;               /// handle from dlsym

    enum m_error (*main)(struct m_module *);
    int (*api_version)(void);
    void (*free)(void);

    int num_callbacks;
    struct m_callback **callbacks;
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

MODULE_EXPORT
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
