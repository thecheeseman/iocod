/**
 * @file mod_example.c
 * 
 * Example module for the module system. Read this for a good idea of how 
 * to begin implementing your own modules.
 */

/*
 * HEADERS
 * 
 * You only need to include "ic_modules.h" to get access to the module system
 * and anything iocod-related. You can import any other headers you want
 * to support functionality in your module.
 * 
 * It is recommended that you _do not_ import <stdio.h> or <stdlib.h> and 
 * instead use the built-in module functions for those tasks (e.g. m_printf,
 * m_alloc, m_realloc, etc). Memory corruption is a very real issue and can
 * cause stability problems, so it's best to use the built-in module supplied
 * functions instead of the standard library where possible.
 */
#include "ic_modules.h"

/*
 * MODULE INFO
 * 
 * While this is not strictly "required" -- it is highly recommended that you
 * provide some information about your module. This way, the module system
 * can know what it's loading and the end-user can also see what modules
 * they have installed/loaded.
 */
#if 0
/* always start with M_INFO_BEGIN() */
M_INFO_BEGIN()

/* name of your module */
M_INFO_NAME("test module")
/* short description */
M_INFO_DESCRIPTION("description goes here")
/* your name or username */
M_INFO_AUTHOR("cheese")
/* your email */
M_INFO_EMAIL("cheese@cheesebox.net")
/* module version*/
M_INFO_VERSION(1, 0 ,0)

/* always end with M_INFO_END() */
M_INFO_END()
#endif

static void module_init(void)
{

}

static void module_free(void)
{

}

/**
 * @brief Main entry point for your module
 * 
 * The function prototype for this must always be:
 * @code
 * m_ptr module_main(m_ptr command, ...)
 * @endcode
 * 
 * This will be called by the module system, with variable arguments depending
 * on what the context is. On start up, the module system will call M_INIT, 
 * allowing you to initialize anything in the module. On system shutdown, 
 * the system will call M_FREE to allow you to release any memory you acquired.
 * 
 * You can implement any number of callbacks here, and the module system will
 * automatically call it. If you do not support a callback, you can just ignore
 * it (not add a case statement for it).
 * 
 * Shown below is a basic example.
 * 
 * @param command callback from the module system
 * @param ... any number of arguments
 * @return 
 */
M_EXPORT
m_ptr module_main(m_ptr command, ...)
{
    /*
     * VARIADIC ARGUMENTS
     * 
     * This call, M_SETUP_ARGS(), is required to enable you to access
     * any of the variadic arguments passed to this function. You can
     * access the arguments via the local variable args[].
     * 
     * You could manually set up varargs if you want but it's easier just
     * to use this macro :)
     */
    M_SETUP_ARGS();

    switch (command) {
    case M_INIT:
        module_init();
        break;
    case M_FREE:
        module_free();
        break;
    default:
        m_debug_printf("unknown system call %d\n", command);
        break;
    }

    return M_OK;
}
