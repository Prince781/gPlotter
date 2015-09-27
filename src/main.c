#include <stdio.h>
#include <math.h>
#include "core/core.h"

int main(int argc, char *argv[]) {
	gp_init();
	g_autoptr(GPFunction) f = gp_function_new("f", "x", "x^3 - 2 + pi");
	g_autoptr(GPFunction) n_abs = gp_native_function_new("abs", (GCallback) &fabs, 1);
	GPVariable *pi = gp_variables_find("pi");

	const gchar *name = gp_function_get_name(f);
	const gchar *vars = gp_function_get_vars(f);
	const gchar *body = gp_function_get_body(f);
	printf("%s(%s) = %s\n", name, vars, body);

	double res = gp_function_eval(f, 3.0);
	printf("%s(3.0) = %lf\n", name, res);

	/* native function */
	name = gp_function_get_name(n_abs);
	vars = gp_function_get_vars(n_abs);
	body = gp_function_get_body(n_abs);
	printf("%s(%s) = %s\n", name, vars, body);
	res = gp_function_eval(n_abs, -3.0);
	printf("%s(-3.0) = %lf\n", name, res);

	/* global variables */
	printf("%s = %lf\n", gp_variable_get_name(pi), gp_variable_get_value(pi));

	gp_deinit();
	return 0;
}
