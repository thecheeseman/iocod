#ifndef IC_TEST_H
#define IC_TEST_H

#include "iocod.h"

#define TEST_MAIN() main(void)

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//#define TEST_MAIN() WINAPI WinMain(HINSTANCE i, HINSTANCE p, PSTR l, INT n)
#else
#include <unistd.h>

//#define TEST_MAIN() main(void)
#endif

#endif /* IC_TEST_H */
