#ifndef NO_COM_H
#define NO_COM_H

#include <stdio.h>
#include <stdlib.h>

#define ERR_DROP 0
#define ERR_FATAL 1
#define com_error(code, ...) { fprintf(stderr, __VA_ARGS__); exit(code); }
#define com_printf(...) fprintf(stderr, __VA_ARGS__)
#define com_print_header(...)
#define com_dprintf(...) fprintf(stderr, __VA_ARGS__)
#define com_printf_logonly(...) fprintf(stderr, __VA_ARGS__)
#define com_warning(...) { fprintf(stderr, "WARNING: "); fprintf(stderr, __VA_ARGS__); }
#define z_malloc malloc
#define z_free free

#define com_startup_variable(var)

#define fs_printf(...)
#define fs_read_file(...) 1

#endif 
