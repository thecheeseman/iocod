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
#define PSAPI_VERSION 1
#include <windows.h>
#include <dbghelp.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <execinfo.h>
#include <sys/resource.h>
#include <sys/utsname.h>

#ifdef IC_PLATFORM_MACOS
#include <mach/mach.h>
#endif
#endif

#include <time.h>

#ifndef IC_PLATFORM_WINDOWS
static char *addr2line(const char *exe, const char *offset)
{
    FILE *fp = popen(va("addr2line -C -i -f -p -e %s %s", exe, offset), "r");
    if (fp == NULL)
        return "??\n";

    static char ret[256];
    while (fgets(ret, sizeof(ret), fp) != NULL);

    pclose(fp);

    return ret;
}
#endif

static void get_processor_info(FILE *out)
{
    // print iocod platform/version stuff first
    // these are statically set when compiled
    fprintf(out, "----------------------------------------\n");
    fprintf(out, "iocod Information:\n");
    fprintf(out, "VERSION:        %s\n", IC_VERSION_STRING);
    fprintf(out, "DATE:           %s\n", __DATE__);
    fprintf(out, "ARCH:           %s\n", IC_PLATFORM_ARCH);

    #ifdef IC_PLATFORM_64BIT
    fprintf(out, "64BIT:          yes\n");
    #else
    fprintf(out, "64BIT:          no\n");
    #endif

    fprintf(out, "OS:             %s\n", IC_PLATFORM_OS);
    #ifdef IC_PLATFORM_OS_VERSION
    fprintf(out, "OS_VERSION:     %s\n", IC_PLATFORM_OS_VERSION);
    #else
    fprintf(out, "OS_VERSION:\n");
    #endif
    fprintf(out, "COMPILER:       %s\n", IC_PLATFORM_COMPILER);
    #ifdef IC_PLATFORM_COMPILER_VER
    fprintf(out, "COMPILER_VER:   %s\n", IC_PLATFORM_COMPILER_VER);
    #else
    fprintf(out, "COMPILER_VER:\n");
    #endif

    fprintf(out, "----------------------------------------\n");
    fprintf(out, "System Information:\n");
    // dynamically grab other system information
    // windows
    #ifdef IC_PLATFORM_WINDOWS
    OSVERSIONINFOEX osvi;
    memset(&osvi, 0, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    GetVersionEx(&osvi);
    
    fprintf(out, "System:         Windows\n");
    fprintf(out, "Release:        %d.%d build %d\n", 
            osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

    fprintf(out, "Version:        ");
    switch (osvi.dwMajorVersion) {
    case 5: // 2000, XP, 2003
        if (osvi.dwMinorVersion == 0)
            fprintf(out, "2000");
        else if (osvi.dwMinorVersion == 1)
            fprintf(out, "XP");
        else if (osvi.dwMinorVersion == 2)
            fprintf(out, "2003/XP 64-bit");
        break;
    case 6: // Vista, 7, 8+
        if (osvi.dwMinorVersion == 0)
            fprintf(out, "Vista/2008");
        else if (osvi.dwMinorVersion == 1)
            fprintf(out, "7/2012");
        else if (osvi.dwMinorVersion == 2)
            fprintf(out, "8/8.1/10/11");
        break;
    default:
        fprintf(out, "unknown");
        break;
    }

    if (osvi.wServicePackMajor != 0) {
        fprintf(out, " SP %d.%d (%s)", osvi.wServicePackMajor,
                osvi.wServicePackMinor, osvi.szCSDVersion);
    }

    fprintf(out, "\n");

    SYSTEM_INFO sysinfo;
    GetNativeSystemInfo(&sysinfo);
    fprintf(out, "Machine:        ");
    switch (sysinfo.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            fprintf(out, "x86_64\n");
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            fprintf(out, "ARM\n");
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            fprintf(out, "IA64\n");
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            fprintf(out, "x86\n");
            break;
        case PROCESSOR_ARCHITECTURE_UNKNOWN:
            fprintf(out, "Unknown\n");
            break;
    }

    char buf[256];
    DWORD bufsize = sizeof(buf);
    if (!GetComputerName(buf, &bufsize))
        fprintf(out, "Domain:         \n");
    else
        fprintf(out, "Domain:         %s\n", buf);

    fprintf(out, "Processors:     %d\n", sysinfo.dwNumberOfProcessors);
    #else
    // linux / macos
    struct utsname uname_info;
    if (uname(&uname_info) != -1) {
        fprintf(out, "System:         %s\n", uname_info.sysname);
        fprintf(out, "Release:        %s\n", uname_info.release);
        fprintf(out, "Version:        %s\n", uname_info.version);
        fprintf(out, "Machine:        %s\n", uname_info.machine);
        fprintf(out, "Node:           %s\n", uname_info.nodename);
    } else {
        fprintf(out, "Could not get uname info\n");
    }
    
    FILE *info = popen(va("cat /proc/cpuinfo | grep processor | wc -l"), "r");
    if (info != NULL) {
        char buf[256];
        while (fgets(buf, sizeof(buf), info) != NULL);
        fprintf(out, "Processors:     %s", buf);
        pclose(info);
    } else {
        fprintf(out, "Processors:     0\n");
    }
    #endif
}

static void get_memory_info(FILE *out)
{
    fprintf(out, "----------------------------------------\n");
    fprintf(out, "Memory Information:\n");
    
    #ifdef IC_PLATFORM_WINDOWS
    MEMORYSTATUSEX statex = {0};
    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    fprintf(out, "MemTotal:       %llu KB\n", statex.ullTotalPhys / 1024);
    fprintf(out, "MemAvail:       %llu KB\n", statex.ullAvailPhys / 1024);

    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    fprintf(out, "WorkingSetSize: %llu KB\n", info.WorkingSetSize / 1024);
    fprintf(out, "PkWrkngSetSize: %llu KB\n", info.PeakWorkingSetSize / 1024);
    fprintf(out, "PagefileUsage:  %llu KB\n", info.PagefileUsage / 1024);
    fprintf(out, "PkPgfileUsage:  %llu KB\n", info.PeakPagefileUsage / 1024);
    #else
    FILE *meminfo = popen(va("cat /proc/meminfo | grep MemTotal"), "r");
    if (meminfo != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), meminfo) != NULL);
        fprintf(out, "%s", line);
        pclose(meminfo);
    } else {
        fprintf(out, "MemTotal:   0\n");
    }

    meminfo = popen(va("cat /proc/meminfo | grep MemAvailable"), "r");
    if (meminfo != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), meminfo) != NULL);
        fprintf(out, "%s", line);
        pclose(meminfo);
    } else {
        fprintf(out, "MemAvail:   0\n");
    }

    #ifdef IC_PLATFORM_MACOS
    struct mach_task_basic_info info;
    mach_msg_type_number_t infocount = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  (task_info_t) &info, &infocount) != KERN_SUCCESS) {
        fprintf(out, "WorkingSetSize: 0\n");
    } else {
        fprintf(out, "WorkingSetSize: %llu KB\n", info.resident_size / 1024);
    }
    #else
    long workingsize = 0L;
    FILE *rss = NULL;
    if ((rss = fopen("/proc/self/statm", "r")) == NULL) {
        fprintf(out, "WorkingSetSize: 0\n");
    } else {
        if (fscanf(rss, "%*s%ld", &workingsize) != 1) {
            fclose(rss);
            fprintf(out, "WorkingSetSize: 0\n");
        }
        fclose(rss);
        fprintf(out, "WorkingSetSize: %ld KB\n", 
                (workingsize * sysconf(_SC_PAGESIZE)) / 1024);
    }
    #endif

    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);

    #ifdef IC_PLATFORM_MACOS
    fprintf(out, "PkWrkngSetSize: %ld KB\n", rusage.ru_maxrss / 1024);
    #else
    fprintf(out, "PkWrkngSetSize: %ld KB\n", rusage.ru_maxrss);
    #endif

    
    #endif
}

/* https://stackoverflow.com/a/1643946 */
static char *last_strstr(const char *haystack, const char *needle)
{
    if (*needle == '\0')
        return (char *) haystack;

    char *result = NULL;
    while (true) {
        char *p = strstr(haystack, needle);
        if (p == NULL)
            break;

        result = p;
        haystack = p + 1;
    }

    return result;
}

static char *shorten_filename(char *filename)
{
    char *match = last_strstr(filename, "src");
    if (match != NULL)
        return match;
    
    #ifdef IC_PLATFORM_WINDOWS
    return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
    #else
    return (strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename);
    #endif 
}

IC_PUBLIC
void sys_backtrace(void)
{   
    time_t t;
    time(&t);

    struct tm *tm_info = localtime(&t);
    char tmbuf[64];
    strftime(tmbuf, sizeof(tmbuf), "%F %T", tm_info);

    FILE *out = stderr;
    FILE *fp = fopen("crash.log", "a+");
    if (fp != NULL)
        out = fp;
    
    fprintf(out, "----------------------------------------"
            "----------------------------------------\n");
    fprintf(out, "Crash Date: %s\n", tmbuf);

    get_processor_info(out);
    get_memory_info(out);
    fprintf(out, "----------------------------------------\n");

    #define STACK_SIZE 63 // needed for XP
    void *stack[STACK_SIZE];

    #ifdef IC_PLATFORM_WINDOWS
    HANDLE process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);
    unsigned short frames = CaptureStackBackTrace(0, STACK_SIZE, stack, NULL);

    fprintf(out, "CaptureStackBackTrace() returned %u frames\n", frames);

    SYMBOL_INFO *symbol = ic_calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    IMAGEHLP_LINE *line = ic_calloc(sizeof(IMAGEHLP_LINE), 1);
    line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

    for (unsigned int i = 0; i < frames; i++) {
        DWORD displacement = 0;
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        SymGetLineFromAddr(process, (DWORD64)(stack[i]), &displacement, line);

        fprintf(out, "./iocodded64(+0x%llx) [0x%llx] %s:%s:%d\n",
                (symbol->Address - symbol->ModBase), symbol->Address,
                shorten_filename(line->FileName), symbol->Name, 
                line->LineNumber);
    }

    ic_free(symbol);
    ic_free(line);
    #else
    int frames = backtrace(stack, STACK_SIZE);
    fprintf(out, "backtrace() returned %d frames\n", frames);

    char **symbols = backtrace_symbols(stack, frames);
    if (symbols != NULL) {
        for (int i = 0; i < frames; i++) {
            fprintf(out, "%s ", symbols[i]);

            char *exe = strtok(symbols[i], "(");
            char *offset = strtok(NULL, ")");
            fprintf(out, "%s", shorten_filename(addr2line(exe, offset)));
        }
    } else {
        fprintf(out, "no symbols returned\n");
    }
    #endif

    fprintf(out, "----------------------------------------"
            "----------------------------------------\n");

    fflush(out);

    if (fp != NULL)
        fclose(fp);
}
