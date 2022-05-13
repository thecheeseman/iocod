#ifndef IC_MODULES_H
#define IC_MODULES_H

#include "../icheaders/iocod.h"

#define M_INTERNAL
#include "m_module.h"

struct m_module {
    struct m_info *info; /// module info passed from module

    size_t id;                  /// internal id that matches dynamic array
    const char *short_name;     /// always equal to filename

    char path[256];             /// exact path to module
    void *handle;               /// handle from dlsym

    m_ptr(*main)(m_ptr command, ...); /// func ptr to `module_main`
    void (*entry)(m_ptr(M_DECL *syscallptr)(m_ptr command, ...));
};

void m_message_hook(enum m_syscall type, const char *fmt, ...);

#define m_printf(...)       m_message_hook(M_PRINTF, __VA_ARGS__)
#define m_warning(...)      m_message_hook(M_WARNING, __VA_ARGS__)
#define m_error(...)        m_message_hook(M_ERROR, __VA_ARGS__)
#define m_error_fatal(...)  m_message_hook(M_ERROR_FATAL, __VA_ARGS__)
#define m_debug_printf(...) m_message_hook(M_DPRINTF, __VA_ARGS__)
#define m_debug_warning(...) m_message_hook(M_DWARNING, __VA_ARGS__)

#define M_EXT "." IC_PLATFORM_DLL

void modules_init(void);
void module_open(const char *name);
void modules_callback(enum m_callback callback, ...);
void modules_free(void);
m_ptr M_DECL m_syscall(m_ptr command, ...);
const char *get_callback_name(int callback);

#endif /* IC_MODULES_H */
