#ifndef _SESSION_H
#define _SESSION_H

#include "ui_function.h"
#include <gtk/gtk.h>
#include <sys/types.h>

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
	ui_function	**ui_f, **f_empty;
	size_t 		nfuncs;
	ssize_t		factive;
};

ssize_t gp_session_add_func(struct gp_session *s, ui_function *f);

ui_function *gp_session_remove_func(struct gp_session *s, ssize_t index);

#endif
