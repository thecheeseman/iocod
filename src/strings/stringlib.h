#ifndef STRINGS_STRINGLIB_H
#define STRINGS_STRINGLIB_H

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#if defined HAVE_STRING_H
#include <string.h>
#endif

#if !defined HAVE_STRDUP 
extern char *strdup(const char *s);
#endif

#if !defined HAVE_STRNDUP
extern char *strndup(const char *s, size_t n);
#endif

#if !defined HAVE_STRLWR
extern char *strlwr(char *s1);
#endif

#if !defined HAVE_STRUPR
extern char *strupr(char *s1);
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

#if !defined HAVE_STRCASECMP
extern int strcasecmp(const char *s1, const char *s2);
#endif

#if !defined HAVE_STRNCASECMP
extern int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

extern void strncpyz(char *dest, const char *src, int destsize);
extern void strncatz(char *dest, int size, const char *src);

extern void snprintfz(char *dest, size_t size, const char *fmt, ...);

#endif /* STRINGS_STRINGLIB_H */
