#ifndef IC_MODULES_H
#define IC_MODULES_H

/*
 * MODULES
 * 
 * The module system begins by scanning the current directory and the 
 * `./modules` directory for any shared objects. It checks if each shared
 * object:
 *      1. can be loaded
 *      2. contains a `module_main` function with this exact prototype:
 *          `enum ic_mod_error module_main(struct ic_module *m)`
 *      3. contains a `module_free` function with this exact prototype:
 *          `void module_free(void)`
 * 
 * If these two steps succeed, the module is then loaded and `module_main` 
 * is called. Module system passes a preallocated empty `struct ic_module *`
 * to the `module_main` function, which then must be set up by the module.
 * 
 * MODULE_MAIN
 * Modules must do the following when `module_main` is called:
 *      1. set give information about the module via `m->set_info()`
 *      2. set up any callbacks via `m->add_callback()`
 *      3. after setup is complete, return MERR_OK
 * 
 * If module does not provide any info, no callbacks will ever be made and
 * a call to `module_free` will immediately occur. 
 */

#ifdef _WIN32
#define MODULE_EXPORT __declspec(dllexport)
#else
#define MODULE_EXPORT extern
#endif

enum ic_mod_error {
    MERR_OK,
    MERR_ALREADY_INIT,
    MERR_FAILED_MALLOC
};

struct ic_mod_callback {
    int a;
};

struct ic_module {
    const char *short_name;     /// always equal to filename
    const char *long_name;      /// name of module
    const char *description;    /// short description
    const char *author;         /// author name
    const char *email;          /// author email
    int version_major;          /// module major version
    int version_minor;          /// module minor version
    int version_patch;          /// module patch version

    void (*set_info)(struct ic_module *m,
                     const char *long_name,
                     const char *description,
                     const char *author,
                     const char *email,
                     int version_major,
                     int version_minor,
                     int version_patch);

    char path[256];             /// exact patch to module
    void *handle;               /// handle from dlsym

    void (*free)(void);

    int num_callbacks;
    struct ic_mod_callback *callbacks;
    enum ic_mod_error (*add_callback)(void);
};

MODULE_EXPORT
void m_printf(const char *fmt, ...);

MODULE_EXPORT
void modules_init(void);

MODULE_EXPORT
void module_open(const char *name);

MODULE_EXPORT
void modules_free(void);

#endif /* IC_MODULES_H */
