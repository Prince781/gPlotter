#include "gp-context.h"
#include "program.h"

struct _GPContextPrivate {
	GHashTable *variables;
	GHashTable *functions;
};

typedef struct _GPContextPrivate GPContextPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GPContext, gp_context, G_TYPE_OBJECT);

static void gp_context_dispose(GObject *gobject) {
	GPContext *self = GP_CONTEXT(gobject);
	GPContextPrivate *priv = gp_context_get_instance_private(self);

	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
	/* dispose references */
	g_hash_table_unref(priv->variables);
	g_hash_table_unref(priv->functions);

	G_OBJECT_CLASS(gp_context_parent_class)->dispose(gobject);
}

static void gp_context_finalize(GObject *gobject) {
	/* TODO: free tables here ? */
	G_OBJECT_CLASS(gp_context_parent_class)->finalize(gobject);
}

static void gp_context_class_init(GPContextClass *klass) {
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

	/* GPContextClass */

	/* GObjectClass */
	gobject_class->dispose = gp_context_dispose;
	gobject_class->finalize = gp_context_finalize;
}

static void gp_context_init(GPContext *self) {
	GPContextPrivate *priv = gp_context_get_instance_private(self);

	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
	priv->variables = g_hash_table_new_full(g_str_hash, g_str_equal,
			NULL, g_object_unref);
	priv->functions = g_hash_table_new_full(g_str_hash, g_str_equal,
			NULL, g_object_unref);
}

GPContext *gp_context_new(void) {
	GObject *obj = g_object_new(GP_TYPE_CONTEXT, NULL);
	return GP_CONTEXT(obj);
}

gboolean gp_context_variables_add(GPContext *self, GPVariable *variable) {
	GPContextPrivate *priv = gp_context_get_instance_private(self);
	const gchar *name = gp_variable_get_name(variable);

	if (g_hash_table_contains(priv->variables, name)) {
		/* TODO: error message */
		return FALSE;
	}
	g_hash_table_insert(priv->variables, name, g_object_ref(variable));
	return TRUE;
}

GPVariable *gp_context_variables_find(GPContext *self, const gchar *name) {
	GPContextPrivate *priv = gp_context_get_instance_private(self);
	return g_hash_table_lookup (priv->variables, name);
}

gboolean gp_context_functions_add(GPContext *self, GPFunction *function) {
	GPContextPrivate *priv = gp_context_get_instance_private(self);
	const gchar *name = gp_function_get_name(function);
	if (g_hash_table_contains(priv->functions, name)) {
		/* TODO: error message */
		return FALSE;
	}
	g_hash_table_insert(priv->functions, name, g_object_ref(function));
	return TRUE;
}

GPFunction *gp_context_functions_find(GPContext *self, const gchar *name) {
	GPContextPrivate *priv = gp_context_get_instance_private(self);
	return g_hash_table_lookup (priv->functions, name);
}
