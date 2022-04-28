#include "../icheaders/iocod.h"
#include "ic_modules.h"

#include "unistd.h"
#include "dlfcn.h"
#include "dirent.h"

static struct ic_module **modules;
static size_t num_modules;

void module_open(const char *name);

#define MODULE_EXT "." IC_PLATFORM_DLL

MODULE_EXPORT
void modules_init(void)
{
    modules = ic_malloc(sizeof(*modules));
    num_modules = 0;

    DIR *dir;
    if ((dir = opendir(".")) != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, "..") == 0 ||
                strcmp(ent->d_name, ".") == 0) {
                continue;
            }

            if (strstr(ent->d_name, MODULE_EXT) != 0)
                module_open(ent->d_name);
        }

        closedir(dir);
    }
}

static void module_set_info(struct ic_module *m, const char *long_name, 
                            const char *description, const char *author, 
                            const char *email, int version_major, 
                            int version_minor, int version_patch)
{
    m->long_name = long_name;
    m->description = description;
    m->author = author;
    m->email = email;
    m->version_major = version_major;
    m->version_minor = version_minor;
    m->version_patch = version_patch;
}

static void init_module_data(struct ic_module *m, void *handle, 
                             const char *module_name, const char *module_path)
{
    m->handle = handle;
    snprintf(m->path, sizeof(m->path), "%s", module_path);

    m->short_name = module_name;
    m->long_name = NULL;
    m->description = NULL;
    m->author = NULL;
    m->email = NULL;
    m->version_major = -1;
    m->version_minor = -1;
    m->version_patch = -1;

    m->set_info = module_set_info;
    m->free = NULL;
}

static bool initialized_properly(struct ic_module *m)
{
    if (m->long_name == NULL || m->description == NULL ||
        m->author == NULL || m->email == NULL ||
        m->version_major == -1 || m->version_minor == -1 ||
        m->version_patch == -1) {
        return false;
    }

    return true;
}

/**
 * @brief Open a module by the given name
 * 
 * This will attempt to look for the module in two places:
 * 1. ./<name>
 * 2. ./modules/<name>
 * 
 * If found, it will look for the symbol `module_main`, and if it exists, 
 * will call it. 
 * 
 * @param[in] name name of the module
*/
MODULE_EXPORT
void module_open(const char *name)
{
    char module_name[64];
    snprintf(module_name, sizeof(module_name), "%s%s", 
             name, strstr(name, MODULE_EXT) ? "" : MODULE_EXT);

    char module_path[256];
    snprintf(module_path, sizeof(module_path), "%s", module_name);

    if (access(module_path, F_OK) == -1) {
        snprintf(module_path, sizeof(module_path), "modules%c%s", PATH_SEP,
                 module_name);

        if (access(module_path, F_OK) == -1) {
            ic_warning("unable to find module '%s'\n", module_name);
            return;
        }
    }

    void *handle = dlopen(module_path, RTLD_LAZY);
    if (handle == NULL) {
        ic_warning("unable to open module '%s': %s\n", module_name, dlerror());
        return;
    }

    dlerror(); // clear any errors

    enum ic_mod_error (*module_main)(struct ic_module *) =
        dlsym(handle, "module_main");
    if (module_main == NULL) {
        #ifdef MODULE_DEBUG
        ic_warning("unable to open module '%s': %s\n", module_name, dlerror());
        #endif
        return;
    }

    // check if this module already exists
    for (size_t i = 0; i < num_modules; i++) {
        if (strcasecmp(modules[i]->path, module_path) == 0) {
            ic_warning("tried to load module '%s' twice\n", module_name);
            return;
        }
    }

    // basic module setup
    struct ic_module m;
    init_module_data(&m, handle, module_name, module_path);

    if (module_main(&m) != MERR_OK) {
        ic_error("module '%s' main failed\n", module_name);
        return;
    }

    if (!initialized_properly(&m)) {
        ic_warning("module '%s' not initialized properly, ignoring\n", 
                   module_name);
        return;
    }

    struct ic_modules **_modules = 
        ic_realloc(modules, sizeof(struct ic_module) * (num_modules + 1));

    if (_modules == NULL) {
        ic_error("error trying to reallocate memory\n");
        return;
    }

    modules = _modules;
    modules[num_modules] = ic_malloc(sizeof(struct ic_module));

    memcpy(modules[num_modules], &m, sizeof(struct ic_module));
    num_modules++;

    ic_printf("opened %s\n", module_name);
}

/**
 * @brief Free all loaded modules
 * 
 * If a given module has its own `free()` defined, that will get called before
 * the shared object is finally closed.
*/
MODULE_EXPORT
void modules_free(void)
{
    for (size_t i = 0; i < num_modules; i++) {
        if (modules[i]->free != NULL)
            modules[i]->free();

        dlclose(modules[i]->handle);
        ic_free(modules[i]);
    }

    ic_free(modules);
}

/**
 * @brief Hook for modules to print things to the console
 * @param fmt format string
 * @param ...
*/
MODULE_EXPORT
void m_printf(const char *fmt, ...)
{
    char msg[1024];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    ic_printf(msg);
}
