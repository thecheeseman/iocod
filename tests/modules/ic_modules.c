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

    return MERR_OK;
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
    m->api_version = -1;
    
    m->num_callbacks = 0;
    m->add_callback = add_callback;

    m->callbacks = ic_malloc(sizeof(*m->callbacks));

    m->main = NULL;
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
    m_debug_printf("  long name:   %s\n", m->long_name);
    m_debug_printf("  description: %s\n", 
                   m->description != NULL ? m->description : 
                   "no description provided");
    m_debug_printf("  author:      %s\n", m->author);
    m_debug_printf("  email:       %s\n", m->email);
    m_debug_printf("  version:     %d.%d.%d\n",
                   M_VERSION_DECODE_MAJOR(m->version),
                   M_VERSION_DECODE_MINOR(m->version),
                   M_VERSION_DECODE_PATCH(m->version));
    int api = m->api_version;
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

    m_debug_printf("  callbacks:\n", m->num_callbacks);
    for (int i = 0; i < m->num_callbacks; i++) {
        m_debug_printf("    %-3d %s\n", i + 1, m->callbacks[i]->name);
    }
}

enum symbol_type {
    STYPE_FUNC,
    STYPE_INT,
    STYPE_CHAR
};

struct symbol_table {
    const char *sym;
    intptr_t offset;
    enum symbol_type type;
    bool optional;
};

static struct symbol_table symtable[] = {
    {
        .sym = "_m_api_version",
        .offset = offsetof(struct m_module, api_version),
        .type = STYPE_INT,
        .optional = false
    },
    {
        .sym = "_m_info_name",
        .offset = offsetof(struct m_module, long_name),
        .type = STYPE_CHAR,
        .optional = false
    },
    {
        .sym = "_m_info_description",
        .offset = offsetof(struct m_module, description),
        .type = STYPE_CHAR,
        .optional = true
    },
    {
        .sym = "_m_info_author",
        .offset = offsetof(struct m_module, author),
        .type = STYPE_CHAR,
        .optional = false
    },
    {
        .sym = "_m_info_email",
        .offset = offsetof(struct m_module, email),
        .type = STYPE_CHAR,
        .optional = false
    },
    {
        .sym = "_m_info_version",
        .offset = offsetof(struct m_module, version),
        .type = STYPE_INT,
        .optional = false
    },
    {
        .sym = "module_main",
        .offset = offsetof(struct m_module, main),
        .type = STYPE_FUNC,
        .optional = false
    },
    {
        .sym = "module_free",
        .offset = offsetof(struct m_module, free),
        .type = STYPE_FUNC,
        .optional = false
    },
};

static size_t symtable_size = ARRAY_SIZE(symtable);

/**
 * @brief Load all the symbols from the given module
 * 
 * Goes through the symbol table and loads the necessary symbols into
 * the module struct automatically
 * 
 * @param m module
*/
static bool load_symbols(struct m_module *m)
{
    for (size_t i = 0; i < symtable_size; i++) {
        struct symbol_table *s = &symtable[i];

        void *sym = dlsym(m->handle, s->sym);
        if (sym == NULL && !s->optional) {
            m_debug_warning("error loading symbol '%s' in '%s':\n    %s\n", 
                            s->sym,
                            m->short_name,
                            dlerror());
            return false;
        }

        intptr_t *ptr = (intptr_t *) ((byte *) m + s->offset);
        switch (s->type) {
        case STYPE_FUNC:
            *ptr = sym;
            break;
        case STYPE_INT:
            *ptr = *(int *) sym;
            break;
        case STYPE_CHAR:
            *ptr = *(const char **) sym;
            break;
        }
    }

    return true;
}

/**
 * @brief Check if the given handle is a valid module
 * 
 * All modules should have a publically exported `m_magic` integer which 
 * equals the magic value of 0x21730. This is an easy check to make sure
 * we have a legit module and aren't trying to load some other shared objects.
 * 
 * @param handle handle to check
*/
static bool valid_module(void *handle)
{
    void *magic = dlsym(handle, "_m_magic");
    if (magic == NULL || *(int *) magic != 0x21730)
        return false;

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

    if (!valid_module(handle)) {
        m_debug_warning("'%s' is not a valid module file, skipping...\n",
                        module_name);
        return;
    }

    /* initialize module data */
    struct m_module m;
    init_module_data(&m, handle, module_name, module_path);

    if (!load_symbols(&m))
        return;

    /* check if if the API version of the module is supported */
    int v = m.api_version;

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
