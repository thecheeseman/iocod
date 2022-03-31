#ifndef SYSTEM_SHARED_H
#define SYSTEM_SHARED_H

#include "iocod.h"
#include "types/bool.h"
#include "types/filehandle.h"
#include <stdint.h>

enum system_event_type {
    SE_NONE = 0,
    SE_KEY,
    SE_CHAR,
    SE_MOUSE,
    SE_JOYSTICK_AXIS,
    SE_CONSOLE,
    SE_PACKET,
    SE_BAD_EVENT
};

struct system_event {
    int time;
    enum system_event_type type;
    int	value;
    int value2;
    int	ptr_length; // bytes of data pointed to by ptr, for journaling
    void *ptr;		// this must be manually freed if not NULL
};

// console.c
void console_show(int level, bool quit_on_close);

// dll.c
void *sys_load_dll(const char *name, char *fqpath,
                   intptr_t(**entrypoint)(intptr_t, ...),
                   intptr_t(*systemcalls)(intptr_t, ...));
void sys_unload_dll(void *dllhandle);

// print.c
void sys_print(const char *msg);

// events
void sys_events_init(void);
void sys_queue_event(int time, enum system_event_type type,
                     int value, int value2, int ptrlength, void *ptr);
struct system_event sys_get_event(void);

// shared
void sys_mkdir(const char *path);
void sys_end_streamed_file(filehandle f);
int sys_milliseconds(void);
char *sys_get_current_user(void);
char *sys_cwd(void);
char *sys_default_base_path(void);
char *sys_default_install_path(void);
char *sys_default_home_path(void);
void sys_set_default_cd_path(const char *path);
char *sys_default_cd_path(void);
bool sys_directory_has_files(const char *ospath);
void sys_list_filtered_files(const char *basedir, char *subdirs, char *filter,
                             char **list, int *numfiles);
char **sys_list_files(const char *directory, const char *extension,
                      char *filter, int *numfiles, bool wantsubs);
void sys_free_file_list(char **list);

//
void sys_do_start_process(char *cmdline);
void sys_exit(int ex);
void sys_quit(void);
void sys_init(void);
void sys_check_for_version(int argc, char *argv[]);
void sys_configure_fpu(void);
void sys_error(const char *error, ...);
void sys_warn(char *warning, ...);

//
extern bool ttycon_on;
extern struct cvar *tty_colors;

// only used for unix -- maybe move there?
//void sys_console_input_shutdown(void);
//void sys_console_input_init(void);
//char *sys_console_input(void); 
//void tty_hide(void);
//void tty_show(void);

#endif /* SYSTEM_SHARED_H */
