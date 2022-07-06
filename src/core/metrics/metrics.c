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
#include <ctype.h>

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <time.h>
#endif

#define MAX_METRICS 64

struct metric {
    char *name;
    char *filename;
    int line;

    enum metric_type type;

    #ifdef IC_PLATFORM_WINDOWS
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    #else
    clock_t start;
    clock_t end;
    #endif
};

static struct metric metrics[MAX_METRICS];

static struct metric *metric_find(const char *name)
{   
    for (int i = 0; i < MAX_METRICS; i++) {
        if (metrics[i].name == NULL)
            continue;

        if (strcasecmp(metrics[i].name, name) == 0)
            return &metrics[i];
    }

    return NULL;
}

void _metric_begin(enum metric_type type, const char *function, 
                   const char *filename, int line)
{
    struct metric *m = metric_find(function);
    if (m == NULL) {
        int index = 0;
        for (; index < MAX_METRICS; index++) {
            if (metrics[index].name == NULL)
                break;
        }

        if (index >= MAX_METRICS) {
            log_trace("reached MAX_METRICS");
            return;
        }

        m = &metrics[index];
        m->name = strdup(function);
        m->filename = strdup(filename);
        m->line = line;
        m->type = type;
    }

    #ifdef IC_PLATFORM_WINDOWS
    QueryPerformanceFrequency(&m->frequency);
    QueryPerformanceCounter(&m->start);
    #else
    m->start = clock();
    #endif
}

void _metric_end(const char *function, int line)
{
    struct metric *m = metric_find(function);
    if (m == NULL) {
        log_trace("can't find metric %s", function);
        return;
    }

    double ms = 0.0f;

    #ifdef IC_PLATFORM_WINDOWS
    QueryPerformanceCounter(&m->end);
    
    LARGE_INTEGER elapsed = { 0 };
    elapsed.QuadPart = m->end.QuadPart - m->start.QuadPart;
    elapsed.QuadPart *= 1000000;
    elapsed.QuadPart /= m->frequency.QuadPart;

    ms = (double) (elapsed.QuadPart) / 1000.0f;
    #else
    m->end = clock();
    ms = (double) ((m->end - m->start) / 1000.0f);
    #endif

    if (m->type == METRIC_ONCE) {
        log_trace("%s (%s:%d-%d) took %.3f ms", m->name, m->filename, m->line,
                  line, ms);

        ic_free(m->name);
        ic_free(m->filename);
    }
}

void metric_cleanup(void)
{
    for (int i = 0; i < MAX_METRICS; i++) {
        ic_free(metrics[i].name);
        ic_free(metrics[i].filename);
    }
}

#if 0
static bool timebase_init = false;

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static LARGE_INTEGER frequency;
static LARGE_INTEGER timebase;

IC_PUBLIC
int32_t sys_milliseconds(void)
{
    if (!timebase_init) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&timebase);
        timebase_init = true;
    }

    LARGE_INTEGER now = { 0 };
    QueryPerformanceCounter(&now);

    LARGE_INTEGER elapsed = { 0 };
    elapsed.QuadPart = now.QuadPart - timebase.QuadPart;
    elapsed.QuadPart *= 1000000;
    elapsed.QuadPart /= frequency.QuadPart;

    return (int32_t) (elapsed.QuadPart / 1000);
}
#else
#include <time.h>

static struct timespec timebase;

IC_PUBLIC
int32_t sys_milliseconds(void)
{
    if (!timebase_init) {
        clock_gettime(CLOCK_REALTIME, &timebase);
        timebase_init = true;
    }

    struct timespec curtime;
    clock_gettime(CLOCK_REALTIME, &curtime);
    double secs = (curtime.tv_sec - timebase.tv_sec) * 1000;
    double ns = (curtime.tv_nsec - timebase.tv_nsec) / 1000000;
    return (int32_t) (secs + ns);
}
#endif
#endif
