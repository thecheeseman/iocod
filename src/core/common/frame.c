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
    if (setjmp(abortframe))
        return; // error

    // com_speeds
    int tbefore_first_events = 0;
    int tbefore_server = 0;
    int tbefore_events = 0;
    int tbefore_client = 0;
    int tafter = 0;

    // write config

    // main event loop
    if (com_speeds->integer > 0)
        tbefore_first_events = sys_milliseconds();

    int min_msec = 1;
    if (com_dedicated->integer > 0)
        min_msec = sv_frame_msec(); 

    int msec = 0;
    static int last_time = 0;
    int tv = 0;
    do {
        if (com_sv_running->integer > 0) {
            // timeval SV = sv_send_queued_packets()
            tv = timeval(min_msec);

            // if (timeval SV < timeval)
            // timeval = timeval sv;
        } else {
            tv = timeval(min_msec);
        }

        if (com_busy_wait->integer > 0 || tv < 1)
            net_sleep(0);
        else
            net_sleep(tv - 1);
    } while (timeval(min_msec));

    last_time = com_frame_time;
    com_frame_time = ev_loop();
    msec = com_frame_time - last_time;

    // run any commands
    cbuf_execute();
    
    //com_frame_msec = msec;
    msec = modify_msec(msec);

    // server side
    if (com_speeds->integer > 0)
        tbefore_server = sys_milliseconds();

    sv_frame(msec);

    if (com_speeds->integer > 0) {
        tafter = sys_milliseconds();
        tbefore_events = tafter;
        tbefore_client = tafter;
    }

    // flush packet queue

    com_frame_number++;
    calculate_fps();
}
