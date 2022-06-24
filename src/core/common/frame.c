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

unsigned int com_frame_number = 0;
int com_frame_time = 0;
int com_frame_msec = 0;

static void calculate_fps(void)
{
    static int lasttime = 0;
    static int lastframecount = 0;

    if (sys_milliseconds() - lasttime >= 1000) {
        lasttime = sys_milliseconds();
        int fps = com_frame_number - lastframecount;
        lastframecount = com_frame_number;

        con_set_title(va(IC_CONSOLE_TITLE " fps: %d", fps));
    }
}

static int modify_msec(int msec)
{
    int clamptime = 5000;
    if (com_dedicated->integer > 0) {
        if (com_sv_running->integer > 0 && msec > 500)
            ic_printf(_("Hitch warning: %i msec frame ime\n"), msec);
    } else {
        if (com_sv_running->integer > 0)
            clamptime = 200; // clients
    }

    if (msec > clamptime)
        msec = clamptime;

    return msec;
}

static int timeval(int msec)
{
    int tv = sys_milliseconds() - com_frame_time;

    if (tv >= msec)
        tv = 0;
    else
        tv = msec - tv;

    return tv;
}

IC_PUBLIC
void com_frame(void)
{
    // abortframe

    int min_msec = 1;
    #ifndef IC_DEDICATED
    if (com_maxfps->integer > 0 && com_dedicated->integer == 0)
        min_msec = 1000 / com_maxfps->integer; 
    #endif

    int msec = 0;
    static int last_time = 0;
    #if 0
    do {
        com_frame_time = ev_loop();
        if (last_time > com_frame_time)
            last_time = com_frame_time;

        msec = com_frame_time - last_time;
    } while (msec < min_msec);
    #endif

    do {
        int tv = timeval(min_msec);
        UNUSED_PARAM(tv);
    } while (timeval(min_msec));

    last_time = com_frame_time;
    com_frame_time = ev_loop();
    msec = com_frame_time - last_time;

    // run any commands
    cbuf_execute();
    
    //com_frame_msec = msec;
    msec = modify_msec(msec);

    // timescale
    // sv_frame

    com_frame_number++;
    calculate_fps();
}
