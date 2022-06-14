#ifndef IOCOD_TYPES_H
#define IOCOD_TYPES_H

/**
 * @defgroup common_types Common Types
 * @brief Various common types used throughout the project.
 * @{
 */
#ifndef _IC_DEFINED_BOOL
#define _IC_DEFINED_BOOL
#ifdef bool
#undef bool
#endif

/**
 * @brief boolean type
 *
 * `bool` must always be the same size as `int` for compatibility (structure
 * field sizes must match), therefore we _cannot_ use `<stdbool.h>`
 * @{
*/
typedef int _boolean;
#define bool _boolean
/** @} */

#define false 0
#define true  1
#endif

#ifndef _IC_DEFINED_BYTE
#define _IC_DEFINED_BYTE

/**
 * @brief Shorthand for `unsigned char`.
*/
typedef unsigned char byte;
#endif

#ifndef _IC_DEFINED_FILEHANDLE
#define _IC_DEFINED_FILEHANDLE

/**
 * @typedef size_t filehandle
 * @brief File handle reference.
 *
 * Strictly speaking, since we probably are never going to handle more than
 * a couple thousand files at a time, `int` sound be fine here (as it was
 * in the original source). In the future this may mean a hash of a given file,
 * rather than just meaning 'nth open file' as it currently does.
*/
typedef size_t filehandle;
#endif

#ifndef _IC_DEFINED_VECTOR
#define _IC_DEFINED_VECTOR

/**
 * @typedef double vec_t
 * @brief Basic vector unit for math.
 *
 * If `IC_VECTOR_FLOAT` is defined, this will instead be `float`.
 */
#ifdef IC_VECTOR_FLOAT
typedef float vec_t;
#else
typedef double vec_t;
#endif

/**
 * @brief Array of vectors.
 * @{
*/
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];
/** @} */
#endif

/** @} */

#endif /* IOCOD_TYPES_H */
