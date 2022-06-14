#ifndef IOCOD_MEMORY_H
#define IOCOD_MEMORY_H

/**
 * @defgroup memory Memory
 * @brief Library replacement memory functions.
 * @{
 */

/**
 * @brief "Safer" library replacement for `malloc`.

 * Always allocates at least 1 byte of memory, and will fatally exit if
 * unable to allocate the memory requested.
 *
 * @param[in] size size in bytes to allocate
*/
IC_MALLOC
IC_PUBLIC
void *ic_malloc(size_t size);

/**
 * @brief "Safer" library replacement for `free`.
 *
 * Always checks that the passed pointer is not NULL before freeing.
 *
 * @param[in] ptr memory to free
*/
IC_PUBLIC
void ic_free(void *ptr);

/**
 * @brief "Safer" library replacement for `calloc`.
 *
 * Always checks that we allocate at least 1 element of size 1. Will fatally
 * exit if unable to allocate the memory requested.
 *
 * @param[in] count number of elements to allocate
 * @param[in] size size of each element
*/
IC_MALLOC
IC_PUBLIC
void *ic_calloc(size_t count, size_t size);

/**
 * @brief "Safer" library replacement for `realloc`.
 *
 * Always checks that we have a valid pointer passed, and if the pointer
 * is NULL, will automatically allocate new memory instead of reallocating.
 * Always makes sure to allocate memory of at least 1 byte. Will fatally exit
 * if it cannot reallocate memory.
 *
 * @param[in] oldptr old memory to reallocate
 * @param[in] size new size of memory
*/
IC_PUBLIC
void *ic_realloc(void *oldptr, size_t size);

/** @} */

#endif /* IOCOD_MEMORY_H */
