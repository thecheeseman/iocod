#include "console.h"

struct console_data console;

IC_PUBLIC
void con_init(void)
{
    con_create();
    con_input_init();
}

IC_PUBLIC
void con_shutdown(void)
{
    con_input_shutdown();
    con_destroy();
}
