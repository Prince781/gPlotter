#ifndef _GP_NATIVE_FUNCTION_H
#define _GP_NATIVE_FUNCTION_H

#include <glib-object.h>
#include <stddef.h>
#include "gp-function.h"

G_BEGIN_DECLS

#define GP_TYPE_NATIVE_FUNCTION (gp_native_function_get_type ())

G_DECLARE_FINAL_TYPE (GPNativeFunction, gp_native_function, GP, NATIVE_FUNCTION,
                      GPFunction);

struct _GPNativeFunction
{
    GPFunction parent_instance;
};

/**
 * gp_native_function_new: (constructor)
 * @name: the name of the function
 * @native: (scope notified): a callback
 * @nparams: number of parameters for the function
 * 
 * Returns: (transfer full): a new #GPNativeFunction
 */
GPFunction *gp_native_function_new (const gchar *name,
                                    GCallback native,
                                    guint nparams);

G_END_DECLS


#endif
