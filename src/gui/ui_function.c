#include "ui_function.h"
#include <stdlib.h>

ui_function *ui_function_new(function *f,
			     double r, double g, double b, double a) {
	ui_function *uif;

	uif = malloc(sizeof(*uif));
	uif->func = f;
	uif->color = malloc(sizeof(GdkRGBA));
	uif->color->red = r;
	uif->color->green = g;
	uif->color->blue = b;
	uif->color->alpha = a;

	return uif;
}

void ui_function_destroy(ui_function *uif) {
	function_destroy(uif->func);
	free(uif->color);
	free(uif);
}
