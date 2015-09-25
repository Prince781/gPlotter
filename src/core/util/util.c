#include "util.h"
#include <stdlib.h>
#include <string.h>

int compare_ptr(const void *p1, const void *p2) {
	if (p1 < p2)
		return -1;
	if (p1 > p2)
		return 1;
	return 0;
}

void *memdup(const void *mem, size_t size) {
	void *new_mem = calloc(1, size);
	memcpy(new_mem, mem, size);
	return new_mem;
}
