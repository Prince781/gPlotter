#include "core.h"
#include "native.h"	/* table of native functions */
#include <glib.h>	/* hash tables */
#include <math.h>	/* constants */

/* variables and functions */

GPContext *gp_context_default = NULL;

#define create_var(name,val) g_autoptr(GPVariable) name = gp_variable_new(#name,val)

void gp_init(void) {
	g_return_if_fail(!GP_IS_CONTEXT(gp_context_default));
	gp_context_default = gp_context_new();
	create_var(pi, M_PI);
	create_var(inf, INFINITY);
	create_var(e, M_E);
	create_var(phi, 1.61803398875);
	gp_variables_add (pi);
	gp_variables_add (inf);
	gp_variables_add (e);
	gp_variables_add (phi);

	for (struct native_func *p = &natives[0]; p->name; ++p) {
		g_autoptr(GPFunction) func = gp_native_function_new(p->name, p->callback, p->params);
		gp_functions_add (func);
	}
}

gboolean gp_variables_add(GPVariable *variable) {
	return gp_context_variables_add (gp_context_default, variable);
}

GPVariable *gp_variables_find(const gchar *name) {
	return gp_context_variables_find (gp_context_default, name);
}

gboolean gp_functions_add(GPFunction *function) {
	return gp_context_functions_add (gp_context_default, function);
}

GPFunction *gp_functions_find(const gchar *name) {
	return gp_context_functions_find (gp_context_default, name);
}

void gp_deinit(void) {
	g_return_if_fail(GP_IS_CONTEXT(gp_context_default));
	g_object_unref (gp_context_default);
	gp_context_default = NULL;
}
