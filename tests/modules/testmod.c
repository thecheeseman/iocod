#include "ic_modules.h"

MODULE_EXPORT
void module_free(void)
{
    
}

static enum m_error test(struct m_module *m, struct m_callback *c)
{
    m_printf("test callback from %s\n", m->short_name);

    return MERR_OK;
}

MODULE_EXPORT
enum ic_module_error module_main(struct m_module *m)
{
    m->long_name = "test module";
    m->author = "cheese";
    m->email = "cheese@cheesebox.net";
    m->version = MODULE_VERSION_ENCODE(1, 0, 0);

    m->add_callback(m, "test", test);

    return MERR_OK;
}
