#ifndef INCLUDE_TYPES_BOOL_H
#define INCLUDE_TYPES_BOOL_H

#ifdef bool
#undef bool
#endif

#ifdef _WIN32
typedef int _boolean;
#define bool _boolean
#else
typedef int boolean;
#define bool boolean
#endif

#define false 0
#define true  1

#endif /* INCLUDE_BOOL_H */
