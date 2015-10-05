#include "gp-variable.h"
#include "program.h"
#include <math.h>   /* constants */

struct _GPVariablePrivate
{
    gchar *name;
    gdouble value;
};

typedef struct _GPVariablePrivate GPVariablePrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GPVariable, gp_variable, G_TYPE_OBJECT);

enum
{
    PROP_0,

    PROP_NAME,
    PROP_VALUE,

    N_PROPERTIES
};

/* pointer to properties definition */
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void gp_variable_set_property (GObject *object,
                                      guint property_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
    GPVariable *self = GP_VARIABLE (object);
    GPVariablePrivate *priv = gp_variable_get_instance_private (self);

    switch (property_id)
        {
        case PROP_NAME:
            g_free (priv->name);
            priv->name = g_value_dup_string (value);
            break;
        case PROP_VALUE:
            priv->value = g_value_get_double (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                               property_id, pspec);
            break;
        }
}

static void gp_variable_get_property (GObject *object,
                                      guint property_id,
                                      GValue *value,
                                      GParamSpec *pspec)
{
    GPVariable *self = GP_VARIABLE (object);
    GPVariablePrivate *priv = gp_variable_get_instance_private (self);

    switch (property_id)
        {
        case PROP_NAME:
            g_value_set_string (value, priv->name);
            break;
        case PROP_VALUE:
            g_value_set_double (value, priv->value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
                                               property_id, pspec);
            break;
        }
}

static void gp_variable_dispose (GObject *gobject)
{
    GPVariable *self = GP_VARIABLE (gobject);

    g_log (GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
    /* dispose references */

    G_OBJECT_CLASS (gp_variable_parent_class)->dispose (gobject);
}

static void gp_variable_finalize (GObject *gobject)
{
    GPVariable *self = GP_VARIABLE (gobject);
    GPVariablePrivate *priv = gp_variable_get_instance_private (self);

    g_free (priv->name);

    G_OBJECT_CLASS (gp_variable_parent_class)->finalize (gobject);
}

static void gp_variable_class_init (GPVariableClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    /* GPFunctionClass */

    /* set properties */
    gobject_class->set_property = gp_variable_set_property;
    gobject_class->get_property = gp_variable_get_property;
    obj_properties[PROP_NAME] =
        g_param_spec_string ("name", "Name of variable",
                             "A variable name, like 'x' or 'pi'",
                             NULL    /* default */,
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_READABLE | G_PARAM_WRITABLE |
                             G_PARAM_STATIC_STRINGS);
    obj_properties[PROP_VALUE] =
        g_param_spec_double ("value", "Value of variable",
                             "A value, like 1.0",
                             -INFINITY, INFINITY, 0.0,
                             G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_READABLE | G_PARAM_WRITABLE |
                             G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties (gobject_class,
                                       N_PROPERTIES, obj_properties);
    gobject_class->dispose = gp_variable_dispose;
    gobject_class->finalize = gp_variable_finalize;
}

static void gp_variable_init (GPVariable *self)
{
    g_log (GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
}

GPVariable *gp_variable_new (const gchar *name, double val)
{
    GObject *obj = g_object_new (GP_TYPE_VARIABLE,
                                 "name", name,
                                 "value", val,
                                 NULL);
    return GP_VARIABLE (obj);
}

const gchar *gp_variable_get_name (GPVariable *self)
{
    GPVariablePrivate *priv = gp_variable_get_instance_private (self);
    return priv->name;
}

gdouble gp_variable_get_value (GPVariable *self)
{
    GPVariablePrivate *priv = gp_variable_get_instance_private (self);
    return priv->value;
}

