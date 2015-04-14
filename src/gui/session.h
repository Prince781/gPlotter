#ifndef _SESSION_H
#define _SESSION_H

#include <gtk/gtk.h>

struct gp_session {
	struct {
		struct {
			GdkRGBA	axes, bg;
		} colors;
		int		show_axes;
		int		label_axes;
		double		xmin, xmax;
		double		ymin, ymax;
	} ui;
};

#endif
