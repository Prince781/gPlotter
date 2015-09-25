#ifndef _GP_FUNCTION_H
#define _GP_FUNCTION_H

#include <glib-object.h>
#include <stddef.h>
#include <stdarg.h>

G_BEGIN_DECLS

#define GP_TYPE_FUNCTION (gp_function_get_type ())

G_DECLARE_DERIVABLE_TYPE(GPFunction, gp_function, GP, FUNCTION, GObject)

struct _GPFunctionClass {
	GObjectClass parent_class;

	/* virtual methods */
	double (*eval)(GPFunction *self, va_list args);
};

GPFunction *gp_function_new(const gchar *name, 
		const gchar *vars, 
		const gchar *body);

const gchar *gp_function_get_name(GPFunction *self);

const gchar *gp_function_get_vars(GPFunction *self);

const gchar *gp_function_get_body(GPFunction *self);

double gp_function_eval(GPFunction *self, ...);

G_END_DECLS

#endif
