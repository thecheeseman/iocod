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

#include "com_local.h"

#ifdef IC_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <stdlib.h>
#include <time.h>

static bool random_bytes(byte *str, size_t len)
{
    bool ret = true;

    #ifdef IC_PLATFORM_WINDOWS
    HCRYPTPROV prov = { 0 };

    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        return false;
    }

    if (!CryptGenRandom(prov, (DWORD) len, (BYTE *) str))
        ret = false;

    CryptReleaseContext(prov, 0);
    #else
    FILE *fp = fopen("/dev/urandom", "r");
    if (fp == NULL)
        return false;

    setvbuf(fp, NULL, _IONBF, 0); // don't buffer reads

    if (fread(str, sizeof(byte), len, fp) != len)
        ret = false;

    fclose(fp);
    #endif

    return ret;
}

void rand_init(void)
{
    unsigned int seed = 0;

    if (random_bytes((byte *) &seed, sizeof(seed))) {
        log_debug(_("System provided random seed %u"), seed);
        srand(seed);
    } else {
        log_debug(_("System couldn't provide random seed, using time(NULL)"));
        srand(time(NULL));
    }
}
