#ifndef INCLUDE_TYPES_BOOL_H
#define INCLUDE_TYPES_BOOL_H

#ifdef bool
#undef bool
#endif

typedef int boolean;
#define bool boolean

#define false 0
#define true  1

#endif /* INCLUDE_BOOL_H */
