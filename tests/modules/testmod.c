#include "ic_modules.h"

M_INFO_START()
M_INFO_NAME("test module")
M_INFO_DESCRIPTION("this is a test module for testing modularity")
M_INFO_AUTHOR("cheese")
M_INFO_EMAIL("cheese@cheesebox.net")
M_INFO_VERSION(1, 0, 0)
M_INFO_END()

static enum m_error test(struct m_module *m, struct m_callback *c)
{
    m_printf("test callback from %s\n", m->short_name);

    return MERR_OK;
}

M_EXPORT
enum ic_module_error module_main(struct m_module *m)
{
    m->add_callback(m, "test", test);

    return MERR_OK;
}

M_EXPORT
void module_free(void)
{

}
