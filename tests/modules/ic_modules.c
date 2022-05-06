#include "../icheaders/iocod.h"
#define MODULE_INTERNAL
#include "ic_modules.h"

#include "unistd.h"
#include "dlfcn.h"
#include "dirent.h"

static struct m_module **modules;
static size_t num_modules;

// TODO: config value
static bool load_different_api_versions = true;

void module_open(const char *name);

#define MODULE_EXT "." IC_PLATFORM_DLL

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

static void init_module_data(struct m_module *m, void *handle, 
                             const char *module_name, const char *module_path)
{
    m->id = num_modules;
    m->short_name = module_name;

    m->handle = handle;
    snprintf(m->path, sizeof(m->path), "%s", module_path);

    m->info = NULL;
    m->main = NULL;
}

static void add_module(struct m_module *m)
{
    struct m_module **_modules =
        ic_realloc(modules, sizeof(struct m_module) * (num_modules + 1));

    if (_modules == NULL) {
        m_error("error trying to reallocate memory\n");
        return;
    }

    modules = _modules;
    modules[num_modules] = ic_malloc(sizeof(struct m_module));

    memcpy(modules[num_modules], m, sizeof(struct m_module));
    num_modules++;
}

static void print_module_info(struct m_module *m)
{
    m_debug_printf("module info: %s\n", m->short_name);

    if (m->info == NULL) {
        m_debug_printf("  no information provided\n");
        return;
    }

    m_debug_printf("  long name:   %s\n", m->info->name);
    m_debug_printf("  description: %s\n", 
                   m->info->description != NULL ? m->info->description :
                   "no description provided");
    m_debug_printf("  author:      %s\n", m->info->author);
    m_debug_printf("  email:       %s\n", m->info->email);
    int version = m->info->version;
    m_debug_printf("  version:     %d.%d.%d\n",
                   M_VERSION_DECODE_MAJOR(version),
                   M_VERSION_DECODE_MINOR(version),
                   M_VERSION_DECODE_PATCH(version));
    int api = m->info->api_version;
    m_debug_printf("  api version: %d.%d.%d",
                   M_VERSION_DECODE_MAJOR(api),
                   M_VERSION_DECODE_MINOR(api),
                   M_VERSION_DECODE_PATCH(api));

    if (M_VERSION_DECODE_MAJOR(api) != M_API_VERSION_MAJOR) {
        m_debug_printf(" -- WARNING: different than system (%s)\n",
                       M_API_VERSION_STRING);
    } else {
        m_debug_printf("\n");
    }
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
            m_debug_warning("unable to find module '%s'\n", module_name);
            return;
        }
    }

    /* check if we've already loaded a module by this path */
    for (size_t i = 0; i < num_modules; i++) {
        if (strcasecmp(modules[i]->path, module_path) == 0) {
            m_debug_warning("tried to load module '%s' twice\n", module_name);
            return;
        }
    }

    void *handle = dlopen(module_path, RTLD_LAZY);
    if (handle == NULL) {
        m_debug_warning("unable to open module '%s': %s\n", module_name, 
                        dlerror());
        return;
    }

    /* initialize module data */
    struct m_module m;
    init_module_data(&m, handle, module_name, module_path);

    /* check for module info */
    m.info = dlsym(handle, "module_info");
    if (m.info != NULL) {
        /* check if if the API version of the module is supported */
        int v = m.info->api_version;

        if (M_VERSION_DECODE_MAJOR(v) != M_API_VERSION_MAJOR) {
            /* major version = incompatible API changes */
            m_warning("module has different API version (%d.%d.%d) " \
                      "than the system API version (%s)",
                      M_VERSION_DECODE_MAJOR(v),
                      M_VERSION_DECODE_MINOR(v),
                      M_VERSION_DECODE_PATCH(v),
                      M_API_VERSION_STRING);

            if (!load_different_api_versions) {
                m_printf("... aborting\n");
                return;
            }

            /* dangerous but if that's what you want */
            m_printf("... loading anyway\n");
        }
    } else {
        m_debug_warning("'%s' did not provide any module information\n",
                        module_name);
    }

    /* check for main entry point */
    m.main = dlsym(handle, "module_main");
    if (m.main == NULL) {
        m_debug_warning("'%s' missing 'module_main', skipping...\n",
                        module_name);
        return;
    }

    /* load main */
    enum m_error err = m.main(M_INIT);
    if (err != M_OK) {
        m_error("module '%s' main failed\n", module_name);
        return;
    }

    add_module(&m);
    print_module_info(&m);
}

/**
 * @brief Free all loaded modules
 * 
 * If a given module has its own `free()` defined, that will get called before
 * the shared object is finally closed.
*/
void modules_free(void)
{
    for (size_t i = 0; i < num_modules; i++) {
        modules[i]->main(M_FREE);

        dlclose(modules[i]->handle);
        ic_free(modules[i]);
    }

    ic_free(modules);
}

/**
 * @brief Hook for modules to print things to the console
 * 
 * @param type hook type
 * @param fmt format string
 * @param ...
 * 
 * @note It is recommended to use the macros `m_printf()`, `m_warning()`, 
 * `m_error()`, and `m_error_fatal()` instead of calling this directly
*/
M_EXPORT
void m_message_hook(enum message_hook_type type, const char *fmt, ...)
{
    char msg[1024];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    switch (type) {
    case MHOOK_PRINTF:      
        ic_printf("%s", msg);
        break;
    case MHOOK_WARNING:     
        ic_warning("%s", msg);
        break;
    case MHOOK_ERROR:       
        ic_error("%s", msg);
        break;
    case MHOOK_ERROR_FATAL: 
        ic_error_fatal("%s", msg);
        break;
    default:
        break;
    }
}
