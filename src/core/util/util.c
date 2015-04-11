#include "util.h"

int compare_ptr(const void *p1, const void *p2) {
	if (p1 < p2)
		return -1;
	if (p1 > p2)
		return 1;
	return 0;
}
