#include "core.h"
#include "native.h"	/* table of native functions */
#include <glib.h>	/* hash tables */
#include <math.h>	/* constants */

/* variables and functions */

GPContext *gp_context_default = NULL;

#define create_var(name,val) g_autoptr(GPVariable) name = gp_variable_new(#name,val)

static void gp_deinit_with_context(GPContext **default_ctx);

void gp_init(void) {
	gp_context_default = gp_context_new();
	gp_init_with_context (&gp_context_default);
}

void gp_deinit(void) {
	gp_deinit_with_context (&gp_context_default);
}

void gp_init_with_context(GPContext **default_ctx) {
	g_return_if_fail(GP_IS_CONTEXT(*default_ctx));
	create_var(pi, M_PI);
	create_var(inf, INFINITY);
	create_var(e, M_E);
	create_var(phi, 1.61803398875);

	gp_context_variables_add (*default_ctx, pi);
	gp_context_variables_add (*default_ctx, inf);
	gp_context_variables_add (*default_ctx, e);
	gp_context_variables_add (*default_ctx, phi);

	for (struct native_func *p = &natives[0]; p->name; ++p) {
		g_autoptr(GPFunction) func = gp_native_function_new(p->name, p->callback, p->params);
		gp_context_functions_add (*default_ctx, func);
	}
}

static void gp_deinit_with_context(GPContext **default_ctx) {
	g_return_if_fail(GP_IS_CONTEXT(*default_ctx));
	g_object_unref (*default_ctx);
	*default_ctx = NULL;
}
