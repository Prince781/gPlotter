#ifndef _GP_FUNCTION_H
#define _GP_FUNCTION_H

#include <glib-object.h>
#include <stdarg.h>

G_BEGIN_DECLS

#define GP_TYPE_FUNCTION (gp_function_get_type ())

G_DECLARE_DERIVABLE_TYPE (GPFunction, gp_function, GP, FUNCTION, GObject);

struct _GPFunctionClass
{
    GObjectClass parent_class;

    /* virtual methods */
    /**
     * gp_function_evalv:
     *
     * @self: a #GPFunction
     * @args: a list of arguments
     * Returns: the result after evaluation
     */
    double (*evalv) (GPFunction *self, va_list args);
};

#include "gp-context.h"

/**
 * gp_function_new: (constructor)
 *
 * @name: the name of the function
 * @vars: (array zero-terminated=1) (element-type char):
 * @body: a description of the function
 * Returns: (transfer full): a new function
 */
GPFunction *gp_function_new (const gchar *name,
                             const gchar *vars,
                             const gchar *body);

/**
 * gp_function_set_context:
 * @self: a #GPFunction
 * @ctx: (nullable): a context
 */
void gp_function_set_context (GPFunction *self, GPContext *ctx);

/**
 * gp_function_get_context:
 *
 * @self: a #GPFunction
 * Returns: (transfer none) (nullable): a #GPContext
 */
GPContext *gp_function_get_context (GPFunction *self);

/**
 * gp_function_get_name:
 *
 * @self: a #GPFunction
 * Returns: (transfer none): the name of the function
 */
const gchar *gp_function_get_name (GPFunction *self);

/**
 * gp_function_get_vars:
 *
 * @self: a #GPFunction
 * Returns: (transfer none) (array zero-terminated=1) (element-type char): a list of chars
 */
const gchar *gp_function_get_vars (GPFunction *self);

/**
 * gp_function_get_body:
 * @self: a #GPFunction
 *
 * Returns: (transfer none): the body of the function
 */
const gchar *gp_function_get_body (GPFunction *self);

/**
 * gp_function_eval:
 *
 * @self: a #GPFunction
 * @...: a list of values, length of #GPFunction.vars
 * Returns: a value
 */
double gp_function_eval (GPFunction *self, ...);

G_END_DECLS

#endif /* _GP_FUNCTION_H */
