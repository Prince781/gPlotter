#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <stddef.h>	/* size_t */

enum function_type { USER, NATIVE, ANY /*used for searching*/ };

struct function {
	char *name;	/* name of function */
	char *vars;
	size_t nvars;	/* number of parameters */
	char *descr;	/* body of function */
	size_t len;
	enum function_type type;
	void *native_fptr;	/* optional function pointer */
	int destroy;	/* whether or not to free() this structure */
};

typedef struct function function;

extern unsigned function_dbg;

function *function_new(const char *name, const char *vars,
		       const char *descr);

function *function_native_new(const char *name, size_t nargs, void *fnative);

/*
 * varargs evaluation
 */
double function_veval(function *f, ...);

double function_eval(function *f, double *vals);

void function_destroy(function *f);

/*
 * initialize library functions
 */
void functions_init(void);

void function_save(const function *f);

function *function_find(const char *name);

int function_remove(function *f);

void functions_uninit(void);
#endif
