#ifndef _GP_CONTEXT_H
#define _GP_CONTEXT_H

#include <glib-object.h>
#include "gp-variable.h"
#include "gp-function.h"

G_BEGIN_DECLS

#define GP_TYPE_CONTEXT (gp_context_get_type ())

G_DECLARE_FINAL_TYPE(GPContext, gp_context, GP, CONTEXT, GObject);

struct _GPContext {
	GObject parent_instance;
};

/**
 * Creates a new execution context.
 */
GPContext *gp_context_new(void);

/**
 * stores @variable internally as a global var
 * @return if adding was successful or not
 */
gboolean gp_context_variables_add(GPContext *self, GPVariable *variable);

/**
 * searches for variable matching @name
 * @return the variable, or NULL if not found
 */
GPVariable *gp_context_variables_find(GPContext *self, const gchar *name);

/**
 * stores @function internally as a global function
 * @return if adding was successful or not
 */
gboolean gp_context_functions_add(GPContext *self, GPFunction *function);

/**
 * searches for a function matching @name
 * @return the function, or NULL if not found
 */
GPFunction *gp_context_functions_find(GPContext *self, const gchar *name);

#endif /* _GP_CONTEXT_H */
