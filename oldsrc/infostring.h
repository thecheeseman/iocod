#ifndef INFOSTRING_H
#define INFOSTRING_H

#ifdef INFOSTRING_STANDALONE
#include "no_com.h"
#endif

#define MAX_INFO_STRING     1024
#define MAX_INFO_KEY        1024
#define MAX_INFO_VALUE      1024

#define BIG_INFO_STRING     8192    // used for system info key only
#define BIG_INFO_KEY        8192
#define BIG_INFO_VALUE      8192

char *info_value_for_key(const char *s, const char *key);
void info_next_pair(const char **head, char *key, char *value);
void info_remove_key(char *s, const char *key);
void info_remove_key_big(char *s, const char *key);
bool info_validate(const char *s);
void info_set_value_for_key(char *s, const char *key, const char *value);
void info_set_value_for_key_big(char *s, const char *key, const char *value);
void info_print(const char *s);

#endif /* INFOSTRING_H */
