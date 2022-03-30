/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

#include <ctype.h>

#include "shared.h"
#include "common.h"

#include "stringlib.h"

#include "common/cbuf.h"
#include "common/error.h"
#include "common/hunk.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "system/system.h"
#include "parse.h"

#define MAX_NUM_ARGVS 50
int com_argc;
char *com_argv[MAX_NUM_ARGVS + 1];

bool com_fully_initialized;

struct cvar *com_developer;
struct cvar *com_maxfps;
struct cvar *com_dedicated;
struct cvar *com_timescale;
struct cvar *com_version;
struct cvar *com_shortversion;
struct cvar *com_developer_script;
struct cvar *com_logfile;
struct cvar *com_fixedtime;
struct cvar *com_viewlog;
struct cvar *com_speeds;
struct cvar *sv_paused;
struct cvar *cl_paused;
struct cvar *com_sv_running;
struct cvar *com_cl_running;
struct cvar *com_introplayed;
struct cvar *com_animcheck;
struct cvar *com_journal;
struct cvar *com_recommendedset;

int com_frame_time;
int com_frame_msec;
int com_frame_number;

// ZONE NONSENSE
#define ZONEID  0x1d4a11
#define MINFRAGMENT 64
struct zonedebug {
    char *label;
    char *file;
    int line;
    int allocsize;
};

struct memblock {
    int size;
    int tag;
    struct memblock *next, *prev;
    int id;
    #ifdef ZONE_DEBUG
    struct zonedebug d;
    #endif
};

struct memzone {
    int size;
    int used;
    struct memblock blocklist;
    struct memblock *rover;
};

struct memzone *mainzone;
struct memzone *smallzone;
// ZONE NONSENSE

#define MAX_CONSOLE_LINES 32
int com_num_consolelines;
char *com_consolelines[MAX_CONSOLE_LINES];

void com_parse_commandline(char *commandline)
{
    com_consolelines[0] = commandline;
    com_num_consolelines = 1;

    while (*commandline != '\0') {
        // look for a + seperator
        if (*commandline == '+' || *commandline == '\n') {
            if (com_num_consolelines == MAX_CONSOLE_LINES)
                return;

            com_consolelines[com_num_consolelines] = commandline + 1;
            com_num_consolelines++;
            *commandline = 0;
        }

        commandline++;
    }
}

bool com_safe_mode(void)
{
    int i;

    for (i = 0; i < com_num_consolelines; i++) {
        cmd_tokenize_string(com_consolelines[i]);

        if (strcasecmp(cmd_argv(0), "safe") == 0 ||
            strcasecmp(cmd_argv(0), "cvar_restart") == 0) {
            com_consolelines[i][0] = 0;
            return true;
        }
    }

    return false;
}

void com_startup_variable(const char *match)
{
    int i;
    char *s;
    struct cvar *cv;

    for (i = 0; i < com_num_consolelines; i++) {
        cmd_tokenize_string(com_consolelines[i]);

        if (strcasecmp(cmd_argv(0), "set") != 0)
            continue;

        s = cmd_argv(1);
        if (match == NULL || strcasecmp(s, match) == 0) {
            cvar_set(s, cmd_argv(2));
            cv = cvar_get(s, "", 0);
            cv->flags |= CVAR_USER_CREATED;
        }
    }
}

bool com_add_startup_commands(void)
{
    int i;
    bool added = false;

    for (i = 0; i < com_num_consolelines; i++) {
        if (com_consolelines[i] == NULL || *com_consolelines[i] == '\0')
            continue;

        if (strncasecmp(com_consolelines[i], "set", 3) != 0)
            added = true;

        cbuf_add_text(com_consolelines[i]);
        cbuf_add_text("\n");
    }

    return added;
}

char *com_string_contains(char *str1, char *str2, bool casesensitive)
{
    int len, i, j;

    len = strlen(str1) - strlen(str2);
    for (i = 0; i <= len; i++, str1++) {
        for (j = 0; str2[j]; j++) {
            if (casesensitive) {
                if (str1[j] != str2[j])
                    break;
            } else {
                if (toupper(str1[j]) != toupper(str2[j]))
                    break;
            }
        }

        if (str2[j] == '\0')
            return str1;
    }

    return NULL;
}

bool com_filter(char *filter, char *name, bool casesensitive)
{
    char buf[MAX_TOKEN_CHARS];
    char *ptr;
    int i;
    bool found;

    while (*filter != '\0') {
        if (*filter == '*') {
            filter++;

            for (i = 0; *filter; i++) {
                if (*filter == '*' || *filter == '?')
                    break;

                buf[i] = *filter;
                filter++;
            }
            buf[i] = '\0';

            if (strlen(buf)) {
                ptr = com_string_contains(name, buf, casesensitive);
                if (ptr == NULL)
                    return false;

                name = ptr + strlen(buf);
            }
        } else if (*filter == '?') {
            filter++;
            name++;
        } else if (*filter == '[' && *(filter + 1) == ']') {
            filter++;
        } else if (*filter == '[') {
            filter++;

            found = false;
            while (*filter != '\0' && !found) {
                if (*filter == ']' && *(filter + 1) != ']')
                    break;

                if (*(filter + 1) == '-' && *(filter + 2) && 
                    (*(filter + 2) != ']' || *(filter + 3) == ']')) {
                    if (casesensitive) {
                        if (*name >= *filter && *name <= *(filter + 2))
                            found = true;
                    } else {
                        if (toupper(*name) >= toupper(*filter) &&
                            toupper(*name) <= toupper(*(filter + 2)))
                            found = true;
                    }
                    filter += 3;
                } else {
                    if (casesensitive) {
                        if (*filter == *name)
                            found = true;
                    } else {
                        if (toupper(*filter) == toupper(*name))
                            found = true;
                    }
                    filter++;
                }
            }
            if (!found)
                return false;

            while (*filter != '\0') {
                if (*filter == ']' && *(filter + 1) != ']')
                    break;

                filter++;
            }
            filter++;
            name++;
        } else {
            if (casesensitive) {
                if (*filter != *name)
                    return false;
            } else {
                if (toupper(*filter) != toupper(*name))
                    return false;
            }
            filter++;
            name++;
        }
    }

    return true;
}

bool com_filter_path(char *filter, char *name, bool casesensitive) 
{
    int i;
    char new_filter[MAX_QPATH];
    char new_name[MAX_QPATH];

    for (i = 0; i < MAX_QPATH - 1 && filter[i]; i++) {
        if (filter[i] == '\\' || filter[i] == ':')
            new_filter[i] = '/';
        else
            new_filter[i] = filter[i];
    }

    new_filter[i] = '\0';
    for (i = 0; i < MAX_QPATH - 1 && name[i]; i++) {
        if (name[i] == '\\' || name[i] == ':')
            new_name[i] = '/';
        else
            new_name[i] = name[i];
    }

    new_name[i] = '\0';
    return com_filter(new_filter, new_name, casesensitive);
}

// MORE ZONE NONSENSE
#ifdef MEMSTATIC
struct memstatic {
    struct memblock b;
    byte mem[2];
};

struct memstatic emptystring = {
    {(sizeof(struct memblock) + 2 + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'\0', '\0'}
};

struct memstatic numberstring[] = {
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'0', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'1', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'2', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'3', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'4', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'5', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'6', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'7', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'8', '\0'}},
    {{(sizeof(struct memstatic) + 3) & ~3, TAG_STATIC, NULL, NULL, ZONEID}, {'9', '\0'}},
};
#endif
// MORE ZONE NONSENSE

char *copy_string(const char *in)
{
    char *out;

    #ifdef MEMSTATIC
    if (!in[0]) //{
       return ((char *) &emptystring) + sizeof(struct memblock);
    } else if (!in[1]) {
        if (in[0] >= '0' && in[0] <= '9')
            return ((char *) &numberstring[in[0] - '0']) + sizeof(struct memblock);
    }
    #endif

    out = z_malloc(strlen(in) + 1);
    com_memset(out, 0, (strlen(in) + 1)); // was sizeof(in)
    strcpy(out, in);
    return out;
}

int com_milliseconds(void)
{
    struct system_event ev;

    // get events and push them until we get a null event with the current time
    do {
        ev = com_get_real_event();

        if (ev.type != SE_NONE)
            com_push_event(&ev);

    } while (ev.type != SE_NONE);

    return ev.time;
}

static int com_modify_msec(int msec)
{
    int clamptime;

    // don't let it scale below 1 msec
    //if (msec < 1 && com_timescale->value)
    //    msec = 1;

    if (com_dedicated->integer > 0) {
        if (msec > 500 && msec < 500000)
            com_printf("Hitch warning: %i msec frame time\n", msec);

        clamptime = 5000;
    } else {
        if (com_sv_running->integer > 0)
            clamptime = 200;
        else
            clamptime = 5000;
    }

    if (msec > clamptime)
        msec = clamptime;

    return msec;
}

static int com_modify_timescale(int msec)
{
    if (com_timescale->value < 0.0)
        cvar_set("timescale", "0");

    if (com_fixedtime->integer > 0)
        msec = com_fixedtime->integer;
    else
        msec = (int)((float) msec * com_timescale->value);

    if (msec < 1 && com_timescale->value != 0.0)
        msec = 1;

    return msec;
}

void com_write_config_to_file(const char *filename)
{
    filehandle f;

    f = fs_fopen_file_write(filename);
    if (f == 0) {
        com_printf("Couldn't write %s.\n", filename);
        return;
    }

    fs_printf(f, "// autogenerated by iocod, do not modify\n");
    key_write_bindings(f);
    cvar_write_variables(f);
    fs_fclose_file(f);
}

void INCOMPLETE com_write_defaults_to_file(const char *filename)
{
    filehandle f;

    f = fs_fopen_file_write(filename);
    if (f == 0) {
        com_printf("Couldn't write %s.\n", filename);
        return;
    }

    fs_printf(f, "// autogenerated by iocod, do not modify\n");
    //key_write_bindings(f);
    cvar_write_defaults(f);
    fs_fclose_file(f);
}

void com_write_configuration(void)
{
    // if we are quiting without fully initializing, make sure
    // we don't write out anything
    if (!com_fully_initialized)
        return;

    if (!(cvar_modified_flags & CVAR_ARCHIVE))
        return;

    cvar_modified_flags &= ~CVAR_ARCHIVE;

    com_write_config_to_file("config_mp_server.cfg");
}

/**
 * @brief 
 * @param  
*/
void com_frame(void)
{
    int msec, minmsec, timescale;
    static int lasttime;

    // timing
    int tbefore_firstevents = 0;
    int tbefore_server = 0;
    int tbefore_events = 0;
    int tbefore_client = 0;
    int tafter = 0;

    if (setjmp(abortframe)) {
        com_error_handler();
        return; // ERR_DROP thrown
    }

    // write the config file if anything changed
    com_write_configuration();

    // if "viewlog" has been modified, show or hide the log console
    if (com_viewlog->modified) {
        if (com_dedicated->integer == 0)
            sys_show_console(com_viewlog->integer, 0);

        com_viewlog->modified = false;
    }

    // fun_0809b048(com_animcheck->integer);
    // just sets a debug value for animations

    //
    // main event loop
    //

    if (com_speeds->integer > 0)
        tbefore_firstevents = sys_milliseconds();

    if (com_maxfps->integer > 0 && com_dedicated->integer == 0)
        minmsec = 1000 / com_maxfps->integer;
    else
        minmsec = 1;

    do {
        com_frame_time = com_event_loop();
        if (lasttime > com_frame_time)
            lasttime = com_frame_time;
        msec = com_frame_time - lasttime;
    } while (msec < minmsec);

    cbuf_execute();

    lasttime = com_frame_time;

    // mess with msec as needed
    com_frame_msec = msec;
    msec = com_modify_msec(msec);

    // mess with timescale
    timescale = com_modify_timescale(msec);

    // 
    // server
    //
    if (com_speeds->integer > 0)
        tbefore_server = sys_milliseconds();

    sv_frame(msec);

    // this is a q3 hold over that is not present in 1.5
    // honestly not really necessary anyway but its here for posterity
    #ifndef CODVERSION1_5
    // if "dedicated" has been modified, start up
    // or shut down the client system.
    // Do this after the server may have started,
    // but before the client tries to auto-connect
    if (com_dedicated->modified) {
        // get the latched value
        cvar_get("dedicated", "0", 0);

        if (com_dedicated->integer == 0) {
            cl_init();
            sys_show_console(com_viewlog->integer, false);
        } else {
            cl_shutdown();
            sys_show_console(1, true);
        }
    }
    #endif

    //
    // client system
    //
    if (com_dedicated->integer == 0) {
        // timing
        if (com_speeds->integer > 0)
            tbefore_events = sys_milliseconds();

        com_event_loop();
        cbuf_execute();

        // timing
        if (com_speeds->integer > 0)
            tbefore_client = sys_milliseconds();

        cl_frame(msec, timescale);

        // timing
        if (com_speeds->integer > 0)
            tafter = sys_milliseconds();

        com_printf("%i %i %i %i %i\n", tbefore_firstevents, tbefore_server,
                   tbefore_events, tbefore_client, tafter);
    }

    com_frame_number++;
}

static void INCOMPLETE com_set_recommended(void)
{
    void *cfg;
    int len;

    len = fs_read_file("configure_mp.csv", &cfg);
    if (len < 0)
        com_error(ERR_FATAL, "Can't find configure_mp.csv");
    /*
    if (len) {
        do {
            uVar7 = (int) *(char *) (iVar6 + local_20) + uVar7 * 0x7a69;
            iVar6 += 1;
        } while (iVar6 < iVar2);
    }
    */
    fs_free_file(cfg);

    // there's a function call here that just returns 0
    // fn_080c6b84();
}

void INCOMPLETE com_init(char *cmdline)
{
    char *s;

    com_print_header("com_init", 40, '-');

    com_printf("%s %s %s\n", Q3_VERSION, PLATFORM_STRING, BUILD_INFO);

    if (setjmp(abortframe)) {
        com_error_handler();
        sys_error("Error during initialization");
    }

    #if defined PLATFORM_WINDOWS
    tty_colors = cvar_get("tty_colors", "0", 0);
    #else
    tty_colors = cvar_get("tty_colors", "1", 0);
    #endif

    com_startup_variable("tty_colors");

    com_init_push_event();
    cvar_init();
    com_parse_commandline(cmdline);

    //swap_init();
    cbuf_init();

    com_init_zone_memory(); 
    cmd_init();

    com_startup_variable(NULL);
    com_startup_variable("developer");

    cl_init_key_commands();
    fs_init_filesystem();

    com_init_journaling();

    cbuf_add_text("exec default_mp.cfg\n");
    cbuf_add_text("exec language.cfg\n");
    cbuf_add_text("exec config_mp_server.cfg\n");
    cbuf_add_text("exec autoexec_mp.cfg\n");

    if (com_safe_mode()) 
        cbuf_add_text("exec safemode_mp_server.cfg\n");

    cbuf_execute();

    com_recommendedset = cvar_get("com_recommendedset", "0", CVAR_ARCHIVE);
    if (com_recommendedset->integer == 0)
        com_set_recommended();
    cvar_set("com_recommendedset", "1");

    // there's another function call here which just returns 0
    // fun_080c6b90();

    com_startup_variable(NULL);

    com_dedicated = cvar_get("dedicated", "2", CVAR_ROM);

    com_init_hunk_memory();

    com_maxfps = cvar_get("com_maxfps", "85", CVAR_ARCHIVE); // not sure this is necessary
    com_developer = cvar_get("developer", "1", CVAR_TEMP);
    com_developer_script = cvar_get("developer_script", "0", CVAR_TEMP);
    com_logfile = cvar_get("logfile", "0", 0);
    com_timescale = cvar_get("timescale", "1", CVAR_CHEAT | CVAR_SYSTEM_INFO);
    com_fixedtime = cvar_get("fixedtime", "0", CVAR_CHEAT);
    com_viewlog = cvar_get("viewlog", "0", CVAR_CHEAT);
    com_speeds = cvar_get("com_speeds", "0", 0);
    sv_paused = cvar_get("sv_paused", "0", CVAR_ROM);
    cl_paused = cvar_get("cl_paused", "0", CVAR_ROM);
    com_sv_running = cvar_get("sv_running", "0", CVAR_ROM);
    com_cl_running = cvar_get("cl_running", "0", CVAR_ROM);
    com_introplayed = cvar_get("com_introplayed", "0", CVAR_ARCHIVE);
    com_animcheck = cvar_get("com_animcheck", "0", 0);

    if (com_dedicated != NULL && com_dedicated->integer > 0)
        cvar_set("viewlog", "1");

    // moved to commands/common_cmd.c
    com_add_commands();

    s = va("%s %s %s", Q3_VERSION, PLATFORM_STRING, BUILD_INFO);
    com_version = cvar_get("version", s, CVAR_ROM);
    com_shortversion = cvar_get("shortversion", "1.1", 
                                CVAR_ROM | CVAR_SERVER_INFO);

    sys_init();
    netchan_init(com_milliseconds() & 0xffff);

    scr_init(com_developer->integer, com_developer_script->integer);

    // these two have to do with script stuff
    // fun_080be758();
    // fun_080c4cf8();

    vm_init();
    sv_init();
    net_open_ip();

    com_dedicated->modified = false;
    if (com_dedicated->integer == 0) {
        cl_init();
        sys_show_console(com_viewlog->integer, 0);
    }

    com_frame_time = com_milliseconds();

    com_add_startup_commands();

    cvar_set("r_uifullscreen", "1");

    cl_start_hunk_users();

    if (com_dedicated->integer == 0) 
        sys_show_console(com_viewlog->integer, 0);

    if (com_dedicated->integer == 0 && com_introplayed->integer == 0) {
        cvar_set("com_introplayed", "1");
        cbuf_add_text("cinematic atvi.roq\n");
        cvar_set("nextmap", "cinematic iw_logo.roq");
    }
    cvar_set("com_statmon", "0");

    com_fully_initialized = true;
    com_print_header("com_init complete", 40, '-');
}

void INCOMPLETE com_shutdown(void)
{
    /*
    fun_0806dec0();
    fun_080c4d28();
    fun_080be838();
    */

    scr_shutdown();
    
    if (logfile != 0) {
        fs_fclose_file(logfile);
        logfile = 0;
    }

    if (com_journalfile != 0) {
        fs_fclose_file(com_journalfile);
        com_journalfile = 0;
    }

    // free any hunk data at this point
    // not really necessary since we're quitting anyway
    // but makes valgrind happy :)
    cvar_shutdown();
    com_hunk_shutdown();
    cmd_shutdown();
    net_shutdown();
}

void INCOMPLETE com_read_cdkey(const char *filename)
{
    UNUSED(filename);
}

void INCOMPLETE com_append_cdkey(const char *filename)
{
    UNUSED(filename);
}
