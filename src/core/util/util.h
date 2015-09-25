#ifndef _U_UTIL_H
#define _U_UTIL_H
#include <stddef.h>

int compare_ptr(const void *p1, const void *p2);

void *memdup(const void *mem, size_t size);

#define dup(ptr) memdup(ptr,sizeof(*ptr))

#endif
