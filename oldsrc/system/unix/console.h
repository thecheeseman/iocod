#ifndef SYSTEM_UNIX_CONSOLE_H
#define SYSTEM_UNIX_CONSOLE_H

#include "cvar/cvar.h"

extern bool ttycon_on;
extern struct cvar *tty_colors;

void console_input_shutdown(void);
void console_input_init(void);
char *console_input(void); 
void tty_hide(void);
void tty_show(void);

#endif /* SYSTEM_UNIX_CONSOLE_H */
