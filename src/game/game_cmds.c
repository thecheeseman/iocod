#include "game_local.h"

char *concat_args(int start) 
{
	int	i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int	len;
	char arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_argc();

	for (i = start; i < c; i++) {
		trap_argv(i, arg, sizeof(arg));

		tlen = strlen(arg);
		if (len + tlen >= MAX_STRING_CHARS - 1)
			break;

		memcpy(line + len, arg, tlen);
		len += tlen;
		if (i != c - 1) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}
