#include <string.h>

#include "testlib.h"
#include "common/memory.h"

char *copy_string(const char *in)
{
	char *out;

	out = malloc(strlen(in) + 1);
    com_memset(out, 0, (strlen(in) + 1)); // was sizeof(in)
	strcpy(out, in);
	return out;
}

void com_printf_runner(enum print_level level, const char *fmt, ...)
{
	va_list argptr;
	char msg[MAX_PRINT_MSG];

	UNUSED(level);

	va_start(argptr, fmt);
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	msg[strlen(msg)] = '\0';
	va_end(argptr);

	printf("%s", msg);
}

void com_printf_dbginfo_runner(const char *file, const char *func, int line,
							   const char *fmt, ...)
{
	va_list argptr;
	char msg[MAX_PRINT_MSG - 6], dbgmsg[MAX_PRINT_MSG];

	va_start(argptr, fmt);
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	msg[strlen(msg)] = '\0';
	va_end(argptr);

	snprintf(dbgmsg, sizeof(dbgmsg), "%s(%s:%d): %s", func, file, line, msg);

	printf("%s", dbgmsg);
}

void com_dprintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAX_PRINT_MSG];

	va_start(argptr, fmt);
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	va_end(argptr);

	printf("%s", msg);
}

void com_error_runner(enum error_code code, const char *file, const char *func,
					  int line, const char *fmt, ...)
{
	va_list argptr;
	//static int lasterrortime, errorcount;
	//int currenttime;
	char err[MAX_PRINT_MSG - 6];
	char com_error_message[MAX_PRINT_MSG];

	UNUSED(code);

	va_start(argptr, fmt);
	vsnprintf(err, sizeof(err), fmt, argptr);
	va_end(argptr);

	snprintf(com_error_message, sizeof(com_error_message), "%s(%s:%d): %s",
			 func, file, line, err);

	fprintf(stderr, "%s", com_error_message);
	_exit(1);
}
