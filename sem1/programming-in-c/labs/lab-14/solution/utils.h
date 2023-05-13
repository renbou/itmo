// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#include <stdlib.h>

/**
 * Function which allocates memory using malloc and raises an error if memory
 * couldn't be allocated
 */
void *xalloc(size_t size);

/**
 * Function which frees memory using free only if the ptr isn't a NULL
 */
void xfree(void *ptr);
