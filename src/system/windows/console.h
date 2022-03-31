#ifndef SYSTEM_WINDOWS_CONSOLE_H
#define SYSTEM_WINDOWS_CONSOLE_H

#include "types/bool.h"

void console_create(void);
void console_destroy(void);
void console_show(int vis_level, bool quit_on_close);
void console_append(const char *message);
void set_error_text(const char *buf);

#endif /* SYSTEM_WINDOWS_CONSOLE_H */
