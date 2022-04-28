#include "ic_modules.h"

static void module_free(void)
{
    m_printf("called from testmod.c module_free");
}

MODULE_EXPORT
enum ic_module_error module_main(struct ic_module *m)
{
    m->set_info(m, 
                "test module",
                "this is a test module for testing functionality",
                "thecheeseman",
                "cheese@cheesebox.net",
                1, 0, 0);

    m->free = module_free;

    return MERR_OK;
}
