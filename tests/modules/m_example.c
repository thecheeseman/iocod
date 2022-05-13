/**
 * @file mod_example.c
 * 
 * Example module for the module system. Read this for a good idea of how 
 * to begin implementing your own modules.
 */

/*
 * HEADERS
 * 
 * You only need to include "m_module.h" to get access to the module system
 * and anything iocod-related. You can import any other headers you want
 * to support functionality in your module.
 * 
 * It is recommended that you _do not_ import <stdio.h> or <stdlib.h> and 
 * instead use the built-in module functions for those tasks (e.g. m_printf,
 * m_alloc, m_realloc, etc). Memory corruption is a very real issue and can
 * cause stability problems, so it's best to use the built-in module supplied
 * functions instead of the standard library where possible.
 */
#include "m_module.h"

/*
 * MODULE INFO
 *
 * While this is not strictly "required" -- it is highly recommended that you
 * provide some information about your module. This way, the module system
 * can know what it's loading and the end-user can also see what modules
 * they have installed/loaded.
 * 
 * Please note that the prototype provided here is required:
 * @code
 * M_EXPORT const struct m_info module_info
 * @endcode
 */
M_EXPORT const struct m_info module_info = {
    .name = "test module",
    .description = "description goes here",
    .author = "cheese",
    .email = "cheese@cheesebox.net",
    .version = M_VERSION_ENCODE(1, 0, 0),
    .api_version = M_API_VERSION
};

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
     * M_SETUP_ARGS() takes one parameter: command
     * 
     * All arguments start at index 1 (args[0] = command)
     */
    M_SETUP_ARGS(command);

    switch (command) {
    /*
     * M_INIT is always called by the system. Use this to initialize any data
     * that you might need.
     */
    case M_INIT:
        module_init();
        break;
    /* 
     * M_FREE is always called by the system on shutdown. Free any data you 
     * allocated or do whatever you need to close your module.
     */
    case M_FREE:
        module_free();
        break;

    case M_TEST_CALLBACK:
        m_printf(va("test callback got %d %d %d\n", command, args[1], args[2]));
        break;

    /*
     * Here you can define any other callbacks you would like to use.
     * See `m_module.h` for the full list
     */

    /*
     * The system will always call the module with every callback that the 
     * system has available. You can choose to leave a warning or simply 
     * ignore any unsupported callbacks
     */
    default:
        m_dwarning(va("%s: unsupported callback %s (%d)\n", module_info.name, 
                      m_callback_name(command), command));
        break;
    }

    return M_OK;
}
