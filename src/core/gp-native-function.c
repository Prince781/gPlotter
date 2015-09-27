#include "gp-native-function.h"
#include "program.h"
#include <string.h>

struct _GPNativeFunctionPrivate {
	GClosure *closure;	/* contains our native function */
	guint params;	/* number of parameters */
};

typedef struct _GPNativeFunctionPrivate GPNativeFunctionPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GPNativeFunction, gp_native_function, GP_TYPE_FUNCTION);

/* virtual methods */
static double gp_native_function_real_eval(GPFunction *self, va_list args);

enum {
	PROP_0,

	PROP_CLOSURE,
	PROP_PARAMS,

	N_PROPERTIES
};

/* pointer to properties definition */
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void gp_native_function_set_property(GObject *object,
		guint property_id,
		const GValue *value,
		GParamSpec *pspec) {
	GPNativeFunction *self = GP_NATIVE_FUNCTION(object);
	GPNativeFunctionPrivate *priv = gp_native_function_get_instance_private(self);
	GClosure *data;

	switch (property_id) {
		case PROP_CLOSURE:
			g_free(priv->closure);
			data = g_value_get_pointer(value);
			priv->closure = data;
			break;
		case PROP_PARAMS:
			priv->params = g_value_get_uint(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object,
					property_id, pspec);
			break;
	}
}

static void gp_native_function_get_property(GObject *object,
		guint property_id,
		GValue *value,
		GParamSpec *pspec) {
	GPNativeFunction *self = GP_NATIVE_FUNCTION(self);
	GPNativeFunctionPrivate *priv = gp_native_function_get_instance_private(self);

	switch (property_id) {
		case PROP_CLOSURE:
			g_value_set_object(value, priv->closure);
			break;
		case PROP_PARAMS:
			g_value_set_uint(value, priv->params);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object,
					property_id, pspec);
			break;
	}
}

static void gp_native_function_dispose(GObject *gobject) {
	GPNativeFunction *self = GP_NATIVE_FUNCTION(gobject);
	GPNativeFunctionPrivate *priv = gp_native_function_get_instance_private(self);

	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
	/* dispose references */
	g_closure_unref(priv->closure);

	G_OBJECT_CLASS(gp_native_function_parent_class)->dispose(gobject);
}

static void gp_native_function_finalize(GObject *gobject) {
	G_OBJECT_CLASS(gp_native_function_parent_class)->finalize(gobject);
}

static void gp_native_function_class_init(GPNativeFunctionClass *klass) {
	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	GPFunctionClass *parent_class = GP_FUNCTION_CLASS(klass);

	/* GPFunctionClass */
	parent_class->eval = gp_native_function_real_eval;

	/* set properties */
	gobject_class->set_property = gp_native_function_set_property;
	gobject_class->get_property = gp_native_function_get_property;
	obj_properties[PROP_CLOSURE] = 
		g_param_spec_pointer("closure", "Function to invoke",
				"Holds a function to invoke later",
				G_PARAM_CONSTRUCT_ONLY |
				G_PARAM_READABLE | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);
	obj_properties[PROP_PARAMS] = 
		g_param_spec_uint("params", "Param number",
				"Number of parameters for function",
				0, ~0u, 0,
				G_PARAM_CONSTRUCT_ONLY |
				G_PARAM_READABLE | G_PARAM_WRITABLE |
				G_PARAM_STATIC_STRINGS);
	g_object_class_install_properties(gobject_class,
			N_PROPERTIES, obj_properties);

	gobject_class->dispose = gp_native_function_dispose;
	gobject_class->finalize = gp_native_function_finalize;
}

static void gp_native_function_init(GPNativeFunction *self) {
	g_log(GP_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, "%s()", __func__);
}

GPFunction *gp_native_function_new(const gchar *name,
		GCallback native,
		guint nparams) {
	// g_return_val_if_fail(nparams > 26, NULL);
	gchar buf[27];
	gchar c = 'a';
	guint i;
	for (i=0; i<nparams; ++i)
		buf[i] = c++;
	buf[i] = '\0';
	GObject *obj = g_object_new(GP_TYPE_NATIVE_FUNCTION,
			"name", name,
			"vars", buf,
			"body", "[native code]",
			"closure", g_cclosure_new(native, NULL, NULL),
			"params", nparams,
			NULL);
	return GP_FUNCTION(obj);
}

static double gp_native_function_real_eval(GPFunction *self, va_list args) {
	g_return_if_fail(GP_IS_NATIVE_FUNCTION(self));
	GPNativeFunction *inst = GP_NATIVE_FUNCTION(self);
	GPNativeFunctionPrivate *priv = gp_native_function_get_instance_private(inst);
	GValue *vals = g_slice_alloc0(sizeof(GValue) * priv->params);
	GValue retval = G_VALUE_INIT;

	for (guint i=0; i<priv->params; ++i) {
		memset(vals+i, 0, sizeof(vals[i]));
		g_value_init(vals+i, G_TYPE_DOUBLE);
		g_value_set_double(vals+i, va_arg(args, double));
	}
	g_value_init(&retval, G_TYPE_DOUBLE);

	/* call our native function */
	g_cclosure_marshal_generic(priv->closure, &retval, priv->params, vals, NULL, NULL);

	g_slice_free1(sizeof(GValue) * priv->params, vals);
	return g_value_get_double(&retval);
}
