#include "core.h"
#include "native.h"	/* table of native functions */
#include <glib.h>	/* hash tables */
#include <math.h>	/* constants */

/* variables and functions */

static GHashTable *variables = NULL;
static GHashTable *functions = NULL;

#define create_var(name,val) g_autoptr(GPVariable) name = gp_variable_new(#name,val)

void gp_init(void) {
	variables = g_hash_table_new_full(g_str_hash, g_str_equal,
			NULL, g_object_unref);
	create_var(pi, M_PI);
	create_var(inf, INFINITY);
	create_var(e, M_E);
	create_var(phi, 1.61803398875);
	gp_variables_add (pi);
	gp_variables_add (inf);
	gp_variables_add (e);
	gp_variables_add (phi);

	functions = g_hash_table_new_full(g_str_hash, g_str_equal,
			NULL, g_object_unref);
	for (struct native_func *p = &natives[0]; p->name; ++p) {
		g_autoptr(GPFunction) func = gp_native_function_new(p->name, p->callback, p->params);
		gp_functions_add (func);
	}
}

gboolean gp_variables_add(GPVariable *variable) {
	const gchar *name = gp_variable_get_name(variable);
	if (g_hash_table_contains(variables, name)) {
		/* TODO: error message */
		return FALSE;
	}
	g_hash_table_insert(variables, name, g_object_ref(variable));
	return TRUE;
}

GPVariable *gp_variables_find(const gchar *name) {
	return g_hash_table_lookup (variables, name);
}

gboolean gp_functions_add(GPFunction *function) {
	const gchar *name = gp_function_get_name(function);
	if (g_hash_table_contains(functions, name)) {
		/* TODO: error message */
		return FALSE;
	}
	g_hash_table_insert(functions, name, g_object_ref(function));
	return TRUE;
}

GPFunction *gp_functions_find(const gchar *name) {
	return g_hash_table_lookup (functions, name);
}

void gp_deinit(void) {
	g_hash_table_destroy(variables);
	g_hash_table_destroy(functions);
}
