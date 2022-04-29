#define MODULE_INTERNAL
#include "ic_modules.h"

extern void modules_init(void);
extern void module_open(const char *name);
extern void modules_free(void);

int main(void)
{
    modules_init();

    //module_open("testmod");

    modules_free();
    return 0;
}
