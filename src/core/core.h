#ifndef _GP_CORE_H
#define _GP_CORE_H

#include "gp-function.h"
#include "gp-variable.h"
#include "gp-native-function.h"

/* variables */

/* initializes gPlotter data */
void gp_init(void);

/**
 * stores @variable internally as a global var
 * @return if adding was successful or not
 */
gboolean gp_variables_add(GPVariable *variable);

/**
 * searches for variable matching @name
 * @return the variable, or NULL if not found
 */
GPVariable *gp_variables_find(const gchar *name);

/**
 * stores @function internally as a global function
 * @return if adding was successful or not
 */
gboolean gp_functions_add(GPFunction *function);

/**
 * searches for a function matching @name
 * @return the function, or NULL if not found
 */
GPFunction *gp_functions_find(const gchar *name);

/* deinitializes gPlotter data */
void gp_deinit(void);

#endif
