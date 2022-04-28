#include "ic_modules.h"

int main(void)
{
    modules_init();

    //module_open("testmod");

    modules_free();
    return 0;
}
