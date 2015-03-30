#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <dyncall.h>	/* dynamic native function calling */
#include "function.h"
#include "ds/stack.h"
#include "util/u_string.h"

#define FUNCTION_TOKEN 4000

#define debugging 1

#define debug(fmt,...) debugging ? printf(fmt, __VA_ARGS__) : -1

#define is_operand(c) (isalnum(c) || c=='.')

#define is_operator(c) (c=='+' || c=='-' || c=='*' || c=='/' || c=='^')

#define is_var(c,vars) (vars != NULL && strchr(vars,c) != NULL)

#define opfunc(c) \
	(c=='+' ? &add : \
	(c=='-' ? &sub : \
	(c=='*' ? &mult : \
	(c=='/' ? &fdiv : \
	(c=='^' ? &pow : \
	(c=='%' ? &fmod : NULL))))))

#define ASSOC_LTR 0
#define ASSOC_RTL 1
#define UNKNOWN   2

#define associativity(c) \
	(c=='+'||c=='-'||c=='*'||c=='-'||c=='%' ? ASSOC_LTR : \
	(c=='!' ? ASSOC_RTL : UNKNOWN))

#define precedence(c) \
	(c=='^' ? 5 : \
	(c=='*' || c=='/' || c=='%' ? 4 : \
	(c=='+' || c=='-' ? 3 : \
	(c==FUNCTION_TOKEN ? 6 : 0))))

#define var(c,f) (strchr((f)->vars,c) - (f)->vars)

static const char *parse_func(const char *s, function **fptr);
static double mult(double, double);
static double fdiv(double, double);
static double add(double, double);
static double sub(double, double);
static double fact(double);
static const char *parse_num(const char *s, double *val);
static int function_native_eval(function *tf, double *vals, double *res);
static int __function_eval(function *f, struct stack_double *operands, double *res);

static void *defined_funcs = NULL;	/* root of binary tree */

#define native_descr "[native]"
#define native_named_func(f,fname,nargs) { fname, NULL, nargs, native_descr, 9, NATIVE, &f, 0 }
#define native_func(f,nargs) { #f, NULL, nargs, native_descr, 9, NATIVE, &f, 0 }

static const function default_funcs[] = {
	native_func(sin,1),
	native_func(asin,1),
	native_func(sinh,1),
	native_func(cos,1),
	native_func(acos,1),
	native_func(cosh,1),
	native_func(tan,1),
	native_func(atan,1),
	native_func(tanh,1),
	native_func(sqrt,1),
	native_func(cbrt,1),
	native_named_func(log10,"log",1),
	native_named_func(log,"ln",1),
	native_named_func(fmax,"max",2),
	native_named_func(fmin,"min",2),
	native_named_func(fabs,"abs",1),
	native_func(erf,1),
	native_func(exp,1)
};

static int func_compare_by_name(const void *f1, const void *f2);

function *function_new(const char *name, const char *vars,
		       const char *descr) {
	function *f;

	f = malloc(sizeof(*f));
	f->name = strdup(name);
	if (vars) {
		f->vars = strdup(vars);
		f->nvars = strlen(f->vars);
	} else {
		f->vars = NULL;
		f->nvars = 0;
	}
	f->descr = strdup(descr);
	f->len = strlen(f->descr);
	f->type = USER;
	f->native_fptr = NULL;
	f->destroy = 1;

	return f;
}

function *function_native_new(const char *name, size_t nargs, void *fnative) {
	function *f;

	f = malloc(sizeof(*f));
	f->name = strdup(name);
	f->vars = NULL;
	f->nvars = nargs;
	f->descr = native_descr;
	f->len = 9;
	f->type = NATIVE;
	f->native_fptr = fnative;
	f->destroy = 1;

	return f;
}

static int function_native_eval(function *tf, double *vals, double *res) {
	int i;

	if (tf->type != NATIVE) {
		fprintf(stderr, "%s: function '%s' is not native\n",
			__func__, tf->name);
		return 0;
	}
	
	DCCallVM *vm = dcNewCallVM(4096);
	dcMode(vm, DC_CALL_C_DEFAULT);
	dcReset(vm);
	for (i=0; i<tf->nvars; ++i)
		dcArgDouble(vm, vals[i]);
	*res = dcCallDouble(vm, tf->native_fptr);
	dcFree(vm);

	return 1;
}

static int __function_eval(function *tf, struct stack_double *operands, 
			   double *res) {
	double *tparams;	// temporary function parameters
	int status = 1;
	int i;

	tparams = malloc(sizeof(*tparams) * tf->nvars);
	debug("%s: evaluating %s(", __func__, tf->name);
	for (i=tf->nvars-1; i>=0; --i)
		tparams[i] = stack_pop(operands);
	if (debugging)
		for (i=0; i<tf->nvars; ++i)
			debug("%lf, ", tparams[i]);
	debug("\b\b%c\n", ')');
	switch(tf->type) {
	case USER:
		*res = function_eval(tf, tparams);
		break;
	case NATIVE:
		status = function_native_eval(tf, tparams, res);
		break;
	default:
		fprintf(stderr, 
			"%s: function '%s' is undefined\n",
			__func__, tf->name);
		status = 0;
		break;
	}
	free(tparams);
	return status;
}

double function_veval(function *f, ...) {
	double *vals = malloc(sizeof(*vals) * f->nvars);
	double ret;
	va_list dp;
	int i;

	va_start(dp, f);
	for (i=0; i<f->nvars; ++i)
		vals[i] = va_arg(dp, double);
	va_end(dp);

	ret = function_eval(f, vals);
	free(vals);
	return ret;
}

double function_eval(function *f, double *vals) {
	struct stack_void_ptr *functions;
	struct stack_int *operators;
	struct stack_double *operands;
	const char *p, *p2;	// description pointers
	function *tf;	// temporary function (if found)
	double val1, val2;	// temporary vals (for operator)
	double res;	// temporary result
	int op;		// temporary operator

	for (int i=0; i<f->nvars; ++i)
		debug("val[%d] = %lf\n", i, vals[i]);

	functions = stack_void_ptr_new(10);
	operators = stack_int_new(10);
	operands = stack_double_new(10);

	for (p=f->descr; *p; ) {
		if ((p2 = parse_func(p, &tf)) > p) {	// if function
			stack_push(functions, tf);
			stack_push(operators, FUNCTION_TOKEN);
			debug("%s: pushing function '%s'\n", __func__, tf->name);
			p = p2;
		} else if ((p2 = parse_num(p, &res)) > p) {	// if number
			stack_push(operands, res);
			p = p2;
		} else if (vals != NULL && is_var(*p,f->vars)) {
			stack_push(operands, vals[var(*p,f)]);
			++p;
		} else if (*p == ',') {	// if function argument separator
			// pop all operators until we encounter left-parens
			while (!stack_empty(operators)
				&& stack_peek(operators) != '(') {
				op = stack_pop(operators);
				if (op == FUNCTION_TOKEN) {
					tf = stack_pop(functions);
					__function_eval(tf, operands, &res);
				} else {
					val2 = stack_pop(operands);
					val1 = stack_pop(operands);
					res = opfunc(op)(val1, val2);
				}
				stack_push(operands, res);
			}
			if (stack_empty(operators)
			&& stack_peek(operators) != '(')
				fprintf(stderr, "%s: misplaced parentheses\n",
					__func__);
			++p;
		} else if (is_operator(*p)) {
			// pop all operators until we meet a '(' or an operator
			// of lower precedence
			debug("%s: found op %c\n", __func__, *p);
			while (!stack_empty(operators)
				&& (op = stack_peek(operators)) != '(' 
				&& precedence(*p) <= precedence(op)) {
				op = stack_pop(operators);
				if (op == FUNCTION_TOKEN) {	
					tf = stack_pop(functions);
					debug("%s: popping %s first\n",
						__func__, tf->name);
					__function_eval(tf, operands, &res);
				} else {
					debug("%s: popping %c first\n",
						 __func__, op);
					val2 = stack_pop(operands);
					val1 = stack_pop(operands);
					res = opfunc(op)(val1, val2);
					debug("%s: pushing %lf %c %lf\n", __func__,
						val1, op, val2);
				}
				stack_push(operands, res);
			}
			debug("%s: pushing %c\n", __func__, *p);
			stack_push(operators, *p);
			++p;
		} else if (*p == '(') {
			stack_push(operators, *p);
			++p;
		} else if (*p == ')') {
			while (!stack_empty(operators)
				&& stack_peek(operators) != '(') {
				op = stack_pop(operators);
				if (op == FUNCTION_TOKEN) {
					tf = stack_pop(functions);
					__function_eval(tf, operands, &res);
				} else {
					val2 = stack_pop(operands);
					val1 = stack_pop(operands);
					res = opfunc(op)(val1, val2);
				}
				stack_push(operands, res);
			}
			if (stack_peek(operators) != '(') {
				fprintf(stderr, "%s: missing '('\n", __func__);
				goto end;
			} else 
				stack_pop(operators);	// pop '('
			++p;
		} else if (isalpha(*p)) {
			fprintf(stderr, 
				"%s: undefined function, expression, or variable '%c'\n",
				__func__, *p);
			++p;
			goto end;
		} else
			++p;
	}

	while (!stack_empty(operators)) {
		op = stack_pop(operators);
		if (op == '(') {
			fprintf(stderr, "%s: unmatched '('\n", __func__);
			break;
		} else if (op == FUNCTION_TOKEN) {
			tf = stack_pop(functions);
			__function_eval(tf, operands, &res);
		} else {
			val2 = stack_pop(operands);
			val1 = stack_pop(operands);
			res = opfunc(op)(val1, val2);
			debug("%s: pushing %lf %c %lf\n", __func__,
			       val1, op, val2);
		}
		stack_push(operands, res);
	}

	res = stack_pop(operands);
end:
	stack_destroy(functions);
	stack_destroy(operands);
	stack_destroy(operators);

	return res;
}

static double mult(double a, double b) {
	return a*b;
}

static double fdiv(double a, double b) {
	return a/b;
}

static double add(double a, double b) {
	return a+b;
}

static double sub(double a, double b) {
	return a-b;
}

static double fact(double n) {
	long v = n;
	if (v < 2) return 1;
	long r = (v % 2) ? 1 + v/2 : 1;	// if odd
	long p = v, a = v, b = v-2;
	while (b > 1) {
		p *= a + b;
		a += b;
		b -= 2;
	}
	return p * r;	// multiply by mid
}

static const char *parse_num(const char *s, double *val) {
	long n = 1;

	*val = 0;
	for (; isdigit(*s); ++s)
		*val = 10 * (*val) + (*s - '0');
	if (*s == '.')
		for (; isdigit(*++s); n*=10)
			*val = 10 * (*val) + (*s - '0');

	*val /= n;
	return s;
}

static const char *parse_func(const char *s, function **fptr) {
	char *func_name;
	size_t func_name_len;

	func_name = get_word(s, &func_name_len);
	if ((*fptr = function_find(func_name)) != NULL) {
		debug("%s: found function '%s'\n", __func__, func_name);
		free(func_name);
		return s + func_name_len;
	}
	free(func_name);
	return s;
}

void function_destroy(function *f) {
	if (!f->destroy) {
		debug("%s: not destroying %s()\n", __func__, f->name);
		return;
	}
	if (f->vars)
		free(f->vars);
	if (f->descr)
		free(f->descr);
	free(f->name);
	free(f);
}

void functions_init(void) {
	const function *f = &default_funcs[0];
	size_t len = sizeof(default_funcs) / sizeof(default_funcs[0]);

	for (; len > 0; --len, ++f) {
		debug("%s: saving function %s -> %s\n", __func__, 
			f->name, f->descr);
		function_save(f);
	}
}

void function_save(const function *f) {
	void *fval = tsearch(f, &defined_funcs, func_compare_by_name);
	if ((*(function **) fval) != f)
		fprintf(stderr, "Could not insert %s\n", *(char **)f);
}

static int func_compare_by_name(const void *f1, const void *f2) {
	return strcmp(((function *)f1)->name, ((function *)f2)->name);
}

function *function_find(const char *name) {
	char *name2 = strdup(name);
	void *found;
	function func = { name2, NULL, 0, NULL, 0, ANY, NULL };
	found = tfind(&func, &defined_funcs, func_compare_by_name);
	free(name2);
	return (found != NULL ? *(function **) found : NULL);
}

int function_remove(function *f) {
	if (defined_funcs == NULL)
		return 0;
	return tdelete(f, &defined_funcs, func_compare_by_name) != NULL;
}

void functions_clear(void) {
	/* TODO */
}
