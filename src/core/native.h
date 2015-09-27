#ifndef _GP_NATIVE_H
#define _GP_NATIVE_H

#include <glib-object.h>

struct native_func {
	const char *name;
	GCallback callback;
	unsigned params;
};

extern struct native_func natives[];

#endif
