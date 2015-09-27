#include "core.h"
#include "program.h"
#include "util/util.h"
#include "util/u_string.h"
#include "ds/stack.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct _GPFunctionPrivate {
	gchar *name;	/* name of function */
	gchar *vars;	/* variables */
	gchar *body;	/* body of function */
};

typedef struct _GPFunctionPrivate GPFunctionPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GPFunction, gp_function, G_TYPE_OBJECT);

/* private methods */
static double gp_function_real_eval(GPFunction *self, va_list args);

/* helper functions */

#define is_operand(c) (isalnum(c) || c=='.')

#define is_operator(c) (c=='+'||c=='-'||c=='*'||c=='/'||c=='^'||c=='%')

#define opfunc(c) \
	(c=='+' ? &add : \
	(c=='-' ? &sub : \
	(c=='*' ? &mult : \
	(c=='/' ? &fdiv : \
	(c=='^' ? &pow : \
	(c=='%' ? &fmod : NULL))))))

enum { ASSOC_LTR, ASSOC_RTL, UNKNOWN };

#define associativity(c) \
	(c=='+'||c=='-'||c=='*'||c=='-'||c=='%' ? ASSOC_LTR : \
	(c=='!'||c=='^' ? ASSOC_RTL : UNKNOWN))

#define FUNCTION_TOKEN 4000

#define precedence(c) \
	(c=='^' ? 5 : \
	(c=='*' || c=='/' || c=='%' ? 4 : \
	(c=='+' || c=='-' ? 3 : \
	(c==FUNCTION_TOKEN ? 6 : 0))))

/**
 * sets val to a new value parsed in the string s
 * returns a position in the string after parsing
 */
static const char *parse_num(const char *s, double *val);

/**
 * sets (*varptr) to point to a global GPVariable if one is found
 * returns a position in the string after parsing
 */
static const char *parse_global_var(const char *s, GPVariable **varptr);

/**
 * sets (*funcptr) to point to a global GPFunction if one is found
 * returns a position in the string after parsing
 */
static const char *parse_global_func(const char *s, GPFunction **funcptr);

static double mult(double, double);
static double fdiv(double, double);
static double add(double, double);
static double sub(double, double);
static double fact(double);

/* end of helper functions */

enum {
	PROP_0,

	PROP_NAME,
	PROP_VARS,
	PROP_BODY,

	N_PROPERTIES
};

/* pointer to properties definition */
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void gp_function_set_property(GObject *object,
		guint property_id,
		const GValue *value,
		GParamSpec *pspec) {
	GPFunction *self = GP_FUNCTION(object);
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);

	switch (property_id) {
		case PROP_NAME:
			g_free(priv->name);
			priv->name = g_value_dup_string(value);
			break;
		case PROP_VARS:
			g_free(priv->vars);
			priv->vars = g_value_dup_string(value);
			break;
		case PROP_BODY:
			g_free(priv->body);
			priv->body = g_value_dup_string(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, 
					property_id, pspec);
			break;
	}
}

static void gp_function_get_property(GObject *object,
		guint property_id,
		GValue *value,
		GParamSpec *pspec) {
	GPFunction *self = GP_FUNCTION(object);
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);

	switch (property_id) {
	case PROP_NAME:
		g_value_set_string(value, priv->name);
		break;
	case PROP_VARS:
		g_value_set_string(value, priv->vars);
		break;
	case PROP_BODY:
		g_value_set_string(value, priv->body);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object,
				property_id, pspec);
		break;
	}
}

static void gp_function_dispose(GObject *gobject) {
	GPFunction *self = GP_FUNCTION(gobject);

	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
	/* dispose references */

	G_OBJECT_CLASS(gp_function_parent_class)->dispose(gobject);
}

static void gp_function_finalize(GObject *gobject) {
	GPFunction *self = GP_FUNCTION(gobject);
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);

	g_free(priv->name);
	g_free(priv->vars);
	g_free(priv->body);

	G_OBJECT_CLASS(gp_function_parent_class)->finalize(gobject);
}

static void gp_function_class_init(GPFunctionClass *klass) {
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

	/* GPFunctionClass */
	klass->eval = gp_function_real_eval;

	/* set properties */
	gobject_class->set_property = gp_function_set_property;
	gobject_class->get_property = gp_function_get_property;
	obj_properties[PROP_NAME] = 
		g_param_spec_string("name", "Name of function",
				"A function name, like 'f' or 'sin'",
				NULL	/* default */,
				G_PARAM_CONSTRUCT_ONLY |
				G_PARAM_READABLE | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_VARS] =
		g_param_spec_string("vars", "Variables of function",
				"A list of variable names to substitute",
				NULL,
				G_PARAM_CONSTRUCT_ONLY |
				G_PARAM_READABLE | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_BODY] =
		g_param_spec_string("body", "Body of function",
				"The body of the function in infix notation",
				NULL,
				G_PARAM_CONSTRUCT_ONLY |
				G_PARAM_READABLE | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);
	g_object_class_install_properties(gobject_class,
			N_PROPERTIES, obj_properties);

	gobject_class->dispose = gp_function_dispose;
	gobject_class->finalize = gp_function_finalize;
}

static void gp_function_init(GPFunction *self) {
	// self->priv = gp_function_get_instance_private(self);
	/* TODO: more code */
	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
}

GPFunction *gp_function_new(const gchar *name,
		const gchar *vars,
		const gchar *body) {
	GObject *obj = g_object_new(GP_TYPE_FUNCTION, 
			"name", name, 
			"vars", vars, 
			"body", body, NULL);
	return GP_FUNCTION(obj);
}

const gchar *gp_function_get_name(GPFunction *self) {
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);
	return priv->name;
}

const gchar *gp_function_get_vars(GPFunction *self) {
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);
	return priv->vars;
}

const gchar *gp_function_get_body(GPFunction *self) {
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);
	return priv->body;
}

double gp_function_eval(GPFunction *self, ...) {
	g_return_if_fail(GP_IS_FUNCTION(self));
	va_list dp;
	double res;

	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
	va_start(dp, self);
	res = GP_FUNCTION_GET_CLASS(self)->eval(self, dp);
	va_end(dp);

	return res;
}

static double gp_function_real_eval(GPFunction *self, va_list args) {
	struct stack_void_ptr *functions;
	struct stack_int *operators;
	struct stack_double *operands;
	const char *p, *p2;	// description pointers
	GPFunction *tf;	// temporary function (if found)
	double val1, val2;	// temporary vals (for operator)
	double res;	// temporary result
	int op;		// temporary operator
	GPFunctionPrivate *priv = gp_function_get_instance_private(self);
	size_t nvars = strlen(priv->vars);
	double *vals = malloc(sizeof(*vals) * nvars);
	GPVariable *global_var = NULL;

	for (size_t i=0; i<nvars; ++i)
		vals[i] = va_arg(args, double);

	functions = stack_void_ptr_new(10);
	operators = stack_int_new(10);
	operands = stack_double_new(10);

	for (p=priv->body; *p; ) {
		if ((p2 = parse_num(p, &res)) > p) {	// if number
			stack_push(operands, res);
			p = p2;
		} else if (strchr(priv->vars, *p) != NULL) {	// if variable
			stack_push(operands, vals[strchr(priv->vars,*p) - priv->vars]);
			++p;
		} else if ((p2 = parse_global_var(p, &global_var)) > p) {
			stack_push(operands, gp_variable_get_value(global_var));
			p = p2;
		} else if (is_operator(*p)) {
			// pop all operators until we meet a '(' or an operator
			// of lower precedence
			g_debug("%s: found op %c\n", __func__, *p);
			while (!stack_empty(operators)
				&& (op = stack_peek(operators)) != '(' 
				&& (precedence(*p) <= precedence(op) 
					|| associativity(*p) != ASSOC_RTL)) {
				op = stack_pop(operators);
				g_debug("%s: popping %c first\n",
					 __func__, op);
				val2 = stack_pop(operands);
				val1 = stack_pop(operands);
				res = opfunc(op)(val1, val2);
				g_debug("%s: pushing %lf %c %lf\n", 
					__func__, val1, op, val2);
				stack_push(operands, res);
			}
			g_debug("%s: pushing %c\n", __func__, *p);
			stack_push(operators, *p);
			++p;
		} else if (*p == '(') {
			stack_push(operators, *p);
			++p;
		} else if (*p == ')') {
			g_debug("%s: found ')'; condensing values\n", __func__);
			while (!stack_empty(operators)
				&& stack_peek(operators) != '(') {
				op = stack_pop(operators);
				val2 = stack_pop(operands);
				val1 = stack_pop(operands);
				res = opfunc(op)(val1, val2);
				stack_push(operands, res);
			}
			if (stack_peek(operators) != '(') {
				g_warning("%s: missing '('\n", __func__);
				goto end;
			} else 
				stack_pop(operators);	// pop '('
			++p;
		} else if (isgraph(*p)) {
			size_t elen;
			char *expr_str = get_word(p, &elen);
			if (elen > 0) {
				g_warning("%s: undefined function, "
					"expression, operator, or variable '%s'\n",
					__func__, expr_str);
				p += elen;
			} else {
				g_warning("%s: undefined function, "
					"expression, operator, or variable '%c'\n",
					__func__, *p);
				++p;
			}
			free(expr_str);
			goto end;
		} else
			++p;
	}

	while (!stack_empty(operators)) {
		op = stack_pop(operators);
		if (op == '(') {
			g_warning("%s: unmatched '('\n", __func__);
			break;
		} else {
			val2 = stack_pop(operands);
			val1 = stack_pop(operands);
			res = opfunc(op)(val1, val2);
			g_debug("res = %lf", res);
			g_debug("%s: pushing %lf %c %lf\n", __func__,
			       val1, op, val2);
		}
		stack_push(operands, res);
	}

	res = stack_empty(operands) ? 0 : stack_pop(operands);
end:
	free(vals);
	stack_destroy(functions);
	stack_destroy(operands);
	stack_destroy(operators);

	return res;
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

static const char *parse_global_var(const char *s, GPVariable **varptr) {
	char *var_name;
	size_t vname_len;

	var_name = get_word(s, &vname_len);
	*varptr = gp_variables_find(var_name);
	free(var_name);
	return s + (*varptr != NULL ? vname_len : 0);
}

static const char *parse_global_func(const char *s, GPFunction **funcptr) {
	char *func_name;
	size_t fname_len;

	func_name = get_word(s, &fname_len);
	*funcptr = gp_functions_find(func_name);
	free(func_name);
	return s + (*funcptr != NULL ? fname_len : 0);
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
