#ifndef STRINGLIB_H
#define STRINGLIB_H

#if defined(HAVE_CONFIG_H) && defined(HAVE_STRING_H)
#include <string.h>
#endif

#if !defined(HAVE_STRDUP)
extern char *strdup(const char *s);
#endif

#if !defined(HAVE_STRNDUP)
extern char *strndup(const char *s, size_t n);
#endif

#if defined(HAVE_CONFIG_H) && defined(HAVE_STRINGS_H)
#include <strings.h>
#endif

#if !defined(HAVE_STRCASECMP)
extern int strcasecmp(const char *s1, const char *s2);
#endif

#if !defined(HAVE_STRNCASECMP)
extern int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

#endif /* STRINGLIB_H */
