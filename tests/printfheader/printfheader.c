#include <stdio.h>
#include <string.h>

/*
 * Format and print a nice header
 */
#define MAX_COLUMNS 80
void com_print_header(const char *text, const char c)
{
	char header[MAX_COLUMNS + 1];
	int i, j, len, lpad, rpad;

	for (i = 0; i < MAX_COLUMNS + 1; i++)
		header[i] = c;
	header[MAX_COLUMNS] = '\0';

	len = strlen(text);
	lpad = (MAX_COLUMNS / 2) - (len / 2);
	rpad = (MAX_COLUMNS / 2) + (len / 2);

	if (lpad < 0)
		lpad = 0;
	if (rpad > MAX_COLUMNS)
		rpad = MAX_COLUMNS;

	header[lpad - 1] = ' ';
	for (i = lpad, j = 0; i < (len + lpad); i++, j++)
		header[i] = text[j];
	header[rpad] = ' ';

	printf("%s\n", header);
}

int main(int argc, char *argv[])
{
	int i;
	if (argc > 1) {
		for (i = 1; i < argc; i++)
			com_print_header(argv[i], '-');
	} else {
		com_print_header("test", '-');
		com_print_header("file system init", '-');
		com_print_header("really long header goes here yeah", '=');
	}

	return 0;
}