#include "../icheaders/iocod.h"
#define MODULE_INTERNAL
#include "ic_modules.h"

#include "unistd.h"
#include "dlfcn.h"
#include "dirent.h"

static struct m_module **modules;
static size_t num_modules;

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

static enum m_error add_callback(struct m_module *m,
                                 const char *name, 
                                 void (*func)(struct m_module *m, 
                                              struct m_callback *c))
{
    struct m_callback **cbs = ic_realloc(m->callbacks,
                                         sizeof(struct m_callback *) *
                                         (m->num_callbacks + 1));
    if (cbs == NULL) {
        ic_error("failed to allocate callback memory\n");
        return MERR_FAILED_MALLOC;
    }

    m->callbacks = cbs;
    m->callbacks[m->num_callbacks] = ic_malloc(sizeof(struct m_callback));
    m->callbacks[m->num_callbacks]->name = name;
    m->callbacks[m->num_callbacks]->func = func;

    m->num_callbacks++;
}

static void init_module_data(struct m_module *m, void *handle, 
                             const char *module_name, const char *module_path)
{
    m->id = num_modules;
    m->short_name = module_name;

    m->handle = handle;
    snprintf(m->path, sizeof(m->path), "%s", module_path);

    m->long_name = NULL;
    m->description = NULL;
    m->author = NULL;
    m->email = NULL;
    m->version = -1;
    
    m->num_callbacks = 0;
    m->add_callback = add_callback;

    m->callbacks = ic_malloc(sizeof(*m->callbacks));

    m->main = NULL;
    m->api_version = NULL;
    m->free = NULL;
}

static bool initialized_properly(struct m_module *m)
{
    if (m->long_name == NULL) {
        m_warning("module '%s' missing 'long_name' field\n", m->short_name);
        return false;
    }

    if (m->author == NULL) {
        m_warning("module '%s' missing 'author' field\n", m->short_name);
        return false;
    }
        
    if (m->email == NULL) {
        m_warning("module '%s' missing 'email' field\n", m->short_name);
        return false;
    }
        
    if (m->version == -1) {
        m_warning("module '%s' missing 'version' field\n", m->short_name);
        return false;
    }

    return true;
}

static bool load_function(struct m_module *m, const char *name)
{
    void *func = dlsym(m->handle, name);
    if (func == NULL) {
        m_debug_warning("unable to open module '%s': %s\n", m->short_name,
                        dlerror());
        return false;
    }

    if (strcmp(name, "module_api_version") == 0)
        m->api_version = func;
    else if (strcmp(name, "module_main") == 0)
        m->main = func;
    else if (strcmp(name, "module_free") == 0)
        m->free = func;
    else
        return false;

    return true;
}

static void add_module(struct m_module *m)
{
    struct ic_modules **_modules =
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
    m_debug_printf("  long name:   %s\n", m->long_name);
    m_debug_printf("  description: %s\n", 
                   m->description != NULL ? m->description : 
                   "no description provided");
    m_debug_printf("  author:      %s\n", m->author);
    m_debug_printf("  email:       %s\n", m->email);
    m_debug_printf("  version:     %d.%d.%d\n",
                   MODULE_VERSION_DECODE_MAJOR(m->version),
                   MODULE_VERSION_DECODE_MINOR(m->version),
                   MODULE_VERSION_DECODE_PATCH(m->version));

    m_debug_printf("  callbacks:\n", m->num_callbacks);
    for (int i = 0; i < m->num_callbacks; i++) {
        m_debug_printf("    %-3d %s\n", i + 1, m->callbacks[i]->name);
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

    /* load required functions */
    if (!load_function(&m, "module_api_version") ||
        !load_function(&m, "module_main") ||
        !load_function(&m, "module_free")) {
        return;
    }

    /* check if if the API version of the module is supported */
    int v = m.api_version();
    if (v > MOD_API_VERSION) {
        m_warning("module has newer API version (%d.%d.%d) than the system (%s)\n",
                  MODULE_VERSION_DECODE_MAJOR(v),
                  MODULE_VERSION_DECODE_MINOR(v),
                  MODULE_VERSION_DECODE_PATCH(v),
                  MOD_API_VERSION_STRING);
        return;
    }

    /* load main */
    enum ic_mod_error err = m.main(&m);
    if (err != MERR_OK) {
        m_error("module '%s' main failed\n", module_name);
        return;
    }

    if (!initialized_properly(&m)) {
        m_debug_warning("module '%s' not initialized properly, ignoring\n", 
                        module_name);
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
        if (modules[i]->free != NULL)
            modules[i]->free();

        if (modules[i]->num_callbacks) {
            for (int j = 0; j < modules[i]->num_callbacks; j++)
                ic_free(modules[i]->callbacks[j]);
        }

        ic_free(modules[i]->callbacks);

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
MODULE_EXPORT
void m_message_hook(enum message_hook_type type, const char *fmt, ...)
{
    char msg[1024];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    switch (type) {
    case MHOOK_PRINTF:      
        ic_printf(msg); 
        break;
    case MHOOK_WARNING:     
        ic_warning(msg); 
        break;
    case MHOOK_ERROR:       
        ic_error(msg); 
        break;
    case MHOOK_ERROR_FATAL: 
        ic_error_fatal(msg); 
        break;
    default:
        break;
    }
}
