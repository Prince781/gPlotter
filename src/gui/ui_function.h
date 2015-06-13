#ifndef _UI_FUNCTION_H
#define _UI_FUNCTION_H

#include <gtk/gtk.h>
#include "../core/function.h"

typedef struct ui_function {
	function *func;
	GdkRGBA	*color;
} ui_function;

#endif
