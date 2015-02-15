#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <stddef.h>

struct function {
	char *descr;
	size_t len;
	char *vars;
	size_t nvars;
};

typedef struct function function;

typedef double (*op_1)(double);
typedef double (*op_2)(double, double);

function *function_new(const char *descr, const char *vars);

double function_eval(function *f);

void function_destroy(function *f);

#endif
