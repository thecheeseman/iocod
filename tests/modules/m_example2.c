/**
 * @file mod_example2.c
 */
#include "m_module.h"

M_INFO_BEGIN()
M_INFO_NAME("test module #2")
M_INFO_DESCRIPTION("more dscription")
M_INFO_AUTHOR("cheese")
M_INFO_EMAIL("cheese@cheesebox.net")
M_INFO_VERSION(3, 0, 0)
M_INFO_END()

static void module_init(void)
{
    
}

static void module_free(void)
{

}

M_EXPORT
m_ptr module_main(m_ptr command, ...)
{
    M_SETUP_ARGS(command);

    switch (command) {
    case M_INIT:
        module_init();
        break;
    case M_FREE:
        module_free();
        break;
    default:
        /* ignore unsupported callbacks */
        break;
    }

    return M_OK;
}
