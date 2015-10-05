#ifndef _GP_CORE_H
#define _GP_CORE_H

#include "gp-context.h"
#include "gp-function.h"
#include "gp-native-function.h"
#include "gp-variable.h"

extern GPContext *gp_context_default;

/* initializes gPlotter data (with a default context) */
void gp_init (void);

/* deinitializes gPlotter data (for a default context) */
void gp_deinit (void);

/**
 * initializes (*default_ctx) with some default
 * global definitions
 */
void gp_init_with_context (GPContext **default_ctx);

/* convenience macros */
#define gp_variables_find(name) gp_context_variables_find (gp_context_default, name)
#define gp_variables_add(var) gp_context_variables_add (gp_context_default, name)

#define gp_functions_find(name) gp_context_functions_find (gp_context_default, name)
#define gp_functions_add(name) gp_context_functions_add (gp_context_default, name)

#endif /* _GP_CORE_H */
