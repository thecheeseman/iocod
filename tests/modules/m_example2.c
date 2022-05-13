/**
 * @file mod_example2.c
 */
#include "m_module.h"

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
