#include "gp-repl.h"
#include "gp-context.h"
#include <glib.h>

GType gp_repl_state_get_type(void) {
	static GType etype = 0;
	if (G_UNLIKELY (etype == 0)) {
		static const GEnumValue values[] = {
			{ GP_REPL_STATE_MAIN, "GP_REPL_STATE_MAIN", "main-state" },
			{ GP_REPL_STATE_EVAL, "GP_REPL_STATE_EVAL", "eval-state" },
			{ 0, NULL, NULL }
		};
		etype = g_enum_register_static (g_intern_static_string ("GPReplState"), values);
	}
	return etype;
}

struct _GPReplPrivate {
	GPContext *ctx;		/* execution context */
	GPReplState state;	/* execution state */
};

/* TODO */
