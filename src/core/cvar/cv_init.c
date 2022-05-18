#include "iocod.h"

struct cvar *cv_cheats;
struct cvar *sv_console_lockout;

extern struct cvar *cvars; /* cv_get.c */
extern void cv_add_commands(void); /* cv_commands.c */

IC_PUBLIC
void cv_init(void)
{
    cv_cheats = cv_get("sv_cheats", "0", CV_ROM | CV_SYSTEM_INFO);
    sv_console_lockout = cv_get("sv_console_lockout", "0", 
                                CV_ROM | CV_SYSTEM_INFO);

    cv_add_commands();
}

IC_PUBLIC
void cv_shutdown(void)
{
    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        if (v->name != NULL)
            ic_free(v->name);

        if (v->string != NULL)
            ic_free(v->string);

        if (v->latched_string != NULL)
            ic_free(v->latched_string);

        if (v->reset_string != NULL)
            ic_free(v->reset_string);
    }
}
