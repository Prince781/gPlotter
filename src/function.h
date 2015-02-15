#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <stddef.h>

struct function {
	char *descr;
	size_t len;
};

typedef struct function function;

typedef double (*op_1)(double);
typedef double (*op_2)(double, double);

double function_eval(function *f);

#endif
