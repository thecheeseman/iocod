/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "iocod/platform.h"

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
