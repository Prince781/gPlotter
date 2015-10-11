#ifndef _GP_CONTEXT_H
#define _GP_CONTEXT_H

#include <glib-object.h>
#include "gp-variable.h"

G_BEGIN_DECLS

#define GP_TYPE_CONTEXT (gp_context_get_type ())

G_DECLARE_FINAL_TYPE (GPContext, gp_context, GP, CONTEXT, GObject);

struct _GPContext {
    GObject parent_instance;
};

#include "gp-function.h"

/**
 * gp_context_new: (constructor)
 *
 * Return: (transfer full): a new execution context.
 */
GPContext *gp_context_new (void);

/**
 * gp_context_variables_add:
 *
 * @self: a #GPContext
 * @variable: a #GPVariable, stored internally as a global var
 * Returns: if adding was successful or not
 */
gboolean gp_context_variables_add (GPContext *self, GPVariable *variable);

/**
 * gp_context_variables_find:
 *
 * @self: a #GPContext
 * @name: the name of a #GPVariable to find
 * Returns: (nullable) (transfer none): the variable, or NULL if not found
 */
GPVariable *gp_context_variables_find (GPContext *self, const gchar *name);

/**
 * gp_context_functions_add:
 *
 * @self: a #GPContext
 * @function: a #GPFunction, stored internally as a global function
 * Returns: if adding was successful or not
 */
gboolean gp_context_functions_add (GPContext *self, GPFunction *function);

/**
 * gp_context_functions_find:
 *
 * @self: a #GPContext
 * @name: the name of the function to search for
 * Returns: (nullable) (transfer none): a #GPFunction, or NULL if not found
 */
GPFunction *gp_context_functions_find (GPContext *self, const gchar *name);

G_END_DECLS

#endif /* _GP_CONTEXT_H */
