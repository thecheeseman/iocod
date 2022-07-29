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

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#include <windows.h>
#include <wincrypt.h>
#else
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <unistd.h>
#endif

#ifdef IC_PLATFORM_WINDOWS
static inline qbool random_bytes_windows(size_t len, void *buf)
{
    HCRYPTPROV prov = { 0 };

    if (!CryptAcquireContextW(&prov, NULL, NULL, PROV_RSA_FULL,
                              CRYPT_VERIFYCONTEXT)) {
        return false;
    }

    if (!CryptGenRandom(prov, (DWORD) len, (BYTE *) buf))
        return false;

    if (!CryptReleaseContext(prov, 0))
        return false;

    return true;
}
#else
/* Linux glibc 2.24 or greater supports this syscall */
#ifdef SYS_getrandom
static inline qbool random_bytes_getrandom(size_t len, void *buf)
{
    long ret = syscall(SYS_getrandom, buf, len, 0);
    ic_printf("syscall returned %d\n", ret);

    return (ret == 0);
}
#else
/* otherwise we can just read from urandom */
static inline qbool random_bytes_dev_random(size_t len, void *buf)
{
    FILE *fp = fopen("/dev/urandom", "r");
    if (fp == NULL)
        return false;

    setvbuf(fp, NULL, _IONBF, 0); // don't buffer reads

    if (fread(buf, sizeof(byte), len, fp) != len)
        ret = false;

    fclose(fp);
}
#endif
#endif

IC_PUBLIC
IC_NON_NULL(2)
qbool sys_random_bytes(size_t len, 
                       _Out_writes_(len) void *buf)
{
    #ifdef IC_PLATFORM_WINDOWS
    return random_bytes_windows(len, buf);
    #else
    #ifdef SYS_getrandom
    return random_bytes_getrandom(len, buf);
    #else
    return random_bytes_dev_random(len, buf);
    #endif
    #endif
}
