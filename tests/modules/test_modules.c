#include "ic_modules.h"

int main(void)
{
    modules_init();

    modules_callback(M_SV_FRAME, 0);

    modules_free();
    return 0;
}
