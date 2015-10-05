#ifndef _GP_VARIABLE_H
#define _GP_VARIABLE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GP_TYPE_VARIABLE (gp_variable_get_type ())

G_DECLARE_FINAL_TYPE (GPVariable, gp_variable, GP, VARIABLE, GObject);

struct _GPVariable
{
    GObject parent_instance;
};

GPVariable *gp_variable_new (const gchar *name, double val);

const gchar *gp_variable_get_name (GPVariable *self);

gdouble gp_variable_get_value (GPVariable *self);

G_END_DECLS

#endif  /* _GP_VARIABLE_H */
