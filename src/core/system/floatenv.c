#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#include <float.h>
#else
#include <fenv.h>
#endif

IC_PUBLIC
void sys_set_floatenv(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    #define FPUCWMASK1 (_MCW_RC | _MCW_EM)
    #define FPUCW (_RC_NEAR | _MCW_EM | _PC_53)

    #if IC_PLATFORM_64BIT
    #define FPUCWMASK	(FPUCWMASK1)
    #else
    #define FPUCWMASK	(FPUCWMASK1 | _MCW_PC)
    #endif
    _controlfp(FPUCW, FPUCWMASK);
    #else
    fesetround(FE_TONEAREST);
    #endif
}
