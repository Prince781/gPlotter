#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "function.h"
#include "ds/stack.h"

/* functions */
#define F_START	4000
#define F_SIN	4001
#define F_COS	4002
#define F_TAN	4003
#define F_ASIN	4004
#define F_ACOS	4005
#define F_ATAN	4006
#define F_ABS	4007
#define F_LN	4010
#define F_LOG10	4011
/* functions */

#define is_operand(c) (isalnum(c) || c == '.')

#define is_operator(c) \
(c == '*' || c == '-' || c == '+' || c == '/' || c == '^')

#define precedence(c) \
-(c == '^' ? 1 : \
(c == '*' || c == '/' ? 2 : \
(c == '+' || c == '-' ? 3 : \
(c > F_START ? -F_START : 4 \
))))

#define _opfunc(c) \
(c == '*' ? &mult : \
(c == '/' ? &div : \
(c == '+' ? &add : \
(c == '-' ? &sub : \
(c == '^' ? &pow : 0 \
)))))

#define _func(c) \
(c == F_SIN ? &sin : \
(c == F_COS ? &cos : \
(c == F_TAN ? &tan : \
(c == F_ASIN ? &asin : \
(c == F_ACOS ? &acos : \
(c == F_ATAN ? &atan : \
(c == F_ABS ? &fabs : \
(c == F_LN ? &log : \
(c == F_LOG10 ? &log10 : 0 \
)))))))))

static const char *parse_func(const char *s, int *op);
static double mult(double, double);
static double div(double, double);
static double add(double, double);
static double sub(double, double);
static const char *parse_num(const char *s, double *val);

double function_eval(function *f) {
	double res = 0;
	const char *p, *p2;
	struct stack_double *operands;
	struct stack_int *operators;
	int o;
	double op1, op2, op3;
	double val = 0;

	operands = stack_double_new(STACK_MINSIZE);
	operators = stack_int_new(STACK_MINSIZE);
	for (p=f->descr; *p; )
		if ((p2 = parse_func(p,&o)) > p) {
			stack_push(operators, o);
			p = p2;
		} else if (is_operand(*p) || *p == '(') {
			if ((p2 = parse_num(p,&val)) > p) {
				stack_push(operands, val);
				p = p2;
			} else {
				stack_push(operands, *p);
				++p;
			}
		} else if (is_operator(*p)) {
			while (!stack_empty(operators) &&
				(precedence((o=stack_peek(operators))) 
				>= precedence(*p) || o > F_START)) {
				o = stack_pop(operators);
				op1 = stack_pop(operands);
				if (o > F_START)
					val = (*_func(o))(op1);
				else {
					op2 = stack_pop(operands);
					val = (*_opfunc(o))(op2, op1);
				}
				stack_push(operands, val);
			}
			stack_push(operators, *p);
			++p;
		} else if (*p == ')') {
// TODO: fix handling of parentheses (treat them as operators, instead)
			while (stack_peek(operands) != '(') {
				o = stack_pop(operators);
				op1 = stack_pop(operands);
				if (o > F_START)
					val = (*_func(o))(op1);
				else {
					op2 = stack_pop(operands);
					val = (*_opfunc(o))(op2, op1);
				}
				op3 = stack_peek(operands);
				if ((int)op3 == '(') {
					stack_pop(operands);
					stack_push(operands, val);
					break;
				} else
					stack_push(operands, val);
			}
			++p;
		} else
			++p;
	while (!stack_empty(operators)) {
		o = stack_pop(operators);
		op1 = stack_pop(operands);
		if (o > F_START)
			val = (*_func(o))(op1);
		else {
			op2 = stack_pop(operands);
			val = (*_opfunc(o))(op2, op1);
		}
		stack_push(operands, val);
	}
	val = stack_pop(operands);
	stack_destroy(operands);
	stack_destroy(operators);
	return val;
}

static double mult(double a, double b) {
	return a*b;
}

static double div(double a, double b) {
	return a/b;
}

static double add(double a, double b) {
	return a+b;
}

static double sub(double a, double b) {
	return a-b;
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

static const char *parse_func(const char *s, int *op) {
	if (strncmp(s, "sin", 3) == 0) {
		*op = F_SIN;
		return s + 3;
	} else if (strncmp(s, "cos", 3) == 0) {
		*op = F_COS;
		return s + 3;
	} else if (strncmp(s, "tan", 3) == 0) {
		*op = F_TAN;
		return s + 3;
	} else if (strncmp(s, "asin", 4) == 0) {
		*op = F_ASIN;
		return s + 4;
	} else if (strncmp(s, "acos", 4) == 0) {
		*op = F_ACOS;
		return s + 4;
	} else if (strncmp(s, "atan", 4) == 0) {
		*op = F_ATAN;
		return s + 4;
	} else if (strncmp(s, "abs", 3) == 0) {
		*op = F_ABS;
		return s + 3;
	} else if (strncmp(s, "log10", 5) == 0) {
		*op = F_LOG10;
		return s + 5;
	} else if (strncmp(s, "ln", 2) == 0) {
		*op = F_LN;
		return s + 2;
	} else if (strncmp(s, "log", 3) == 0) {
		*op = F_LN;
		return s + 3;
	} else
		return s;
}
