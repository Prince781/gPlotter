#ifndef _UI_FUNCTION_H
#define _UI_FUNCTION_H

#include "../core/function.h"
#include <gtk/gtk.h>

struct ui_function {
	function	*func;
	GdkRGBA		*color;
};

typedef struct ui_function ui_function;

#endif
