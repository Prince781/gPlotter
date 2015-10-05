#include "gp-repl.h"
#include "gp-context.h"
#include "program.h"
#include <glib.h>

GType gp_repl_state_get_type (void)
{
    static GType etype = 0;
    if (G_UNLIKELY (etype == 0))
        {
            static const GEnumValue values[] =
            {
                { GP_REPL_STATE_MAIN, "GP_REPL_STATE_MAIN", "main-state" },
                { GP_REPL_STATE_EVAL, "GP_REPL_STATE_EVAL", "eval-state" },
                { 0, NULL, NULL }
            };
            etype = g_enum_register_static (g_intern_static_string ("GPReplState"), values);
        }
    return etype;
}

struct _GPReplPrivate
{
    GPContext *ctx;     /* execution context */
    GPReplState state;  /* execution state */
};

typedef struct _GPReplPrivate GPReplPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GPRepl, gp_repl, G_TYPE_OBJECT);

static void gp_repl_dispose (GObject *gobject)
{
    GPRepl *self = GP_REPL (gobject);
    GPReplPrivate *priv = gp_repl_get_instance_private (self);

    g_log (GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
    /* dispose references */
    g_object_unref (priv->ctx);

    G_OBJECT_CLASS (gp_repl_parent_class)->dispose (gobject);
}

static void gp_repl_finalize (GObject *gobject)
{
    G_OBJECT_CLASS (gp_repl_parent_class)->finalize (gobject);
}

static void gp_repl_class_init (GPReplClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose = gp_repl_dispose;
    gobject_class->finalize = gp_repl_finalize;
}

static void gp_repl_init (GPRepl *self)
{
    GPReplPrivate *priv;

    g_log (GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
    priv = gp_repl_get_instance_private (self);
    priv->ctx = gp_context_new();
    priv->state = GP_REPL_STATE_MAIN;
}

GPRepl *gp_repl_new (void)
{
    GObject *obj = g_object_new (GP_TYPE_REPL, NULL);
    return GP_REPL (obj);
}
