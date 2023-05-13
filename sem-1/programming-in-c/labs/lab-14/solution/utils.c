// ITMO 09.03.02 2020
// Artem Mikheev M3101
// Apache License

#include <stdlib.h>
#include "utils.h"
#include "errors.h"

void *xalloc(size_t size) {
	void *result = malloc(size);
	if (result == NULL) {
		PUT_ERROR("Unable to allocate memory. Program must quit.");
		exit(-1);
	}
	return result;
}

void xfree(void *ptr) {
	if (ptr != NULL) {
		free(ptr);
	}
}
