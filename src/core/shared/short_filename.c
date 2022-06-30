#include "iocod.h"

/* https://stackoverflow.com/a/1643946 */
static char *last_strstr(const char *haystack, const char *needle)
{
    if (*needle == '\0')
        return (char *) haystack;

    char *result = NULL;
    while (true) {
        char *p = strstr(haystack, needle);
        if (p == NULL)
            break;

        result = p;
        haystack = p + 1;
    }

    return result;
}

IC_PUBLIC
char *ic_short_filename(char *filename)
{
    char *match = last_strstr(filename, "src");
    if (match != NULL)
        return match;

    #ifdef IC_PLATFORM_WINDOWS
    return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
    #else
    return (strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename);
    #endif 
}
