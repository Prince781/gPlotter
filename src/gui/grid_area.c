#include <stdio.h>
#include "grid_area.h"
#include "gplotter_app_window.h"

gboolean grid_draw2d(GtkWidget *widget, cairo_t *cr, gpointer data) {
	unsigned width, height;
	struct gp_session *session;
	char buf[128];
	double center_x, center_y;
	double diff_x, diff_y;
	
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);
	session = GPLOTTER_APP_WINDOW(data)->session;
	diff_x = session->ui.xmax - session->ui.xmin;
	diff_y = session->ui.ymax - session->ui.ymin;

	center_x = -(session->ui.xmin / diff_x) * width;
	center_y = -(session->ui.ymin / diff_y) * height;

	cairo_save(cr);

	// fill
	gdk_cairo_set_source_rgba(cr, &session->ui.colors.bg);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_fill(cr);

	// draw axes
	if (session->ui.show_axes) {
		gdk_cairo_set_source_rgba(cr, &session->ui.colors.axes);
		cairo_set_line_width(cr, 1);
		cairo_move_to(cr, 0, height/2);
		cairo_line_to(cr, width, height/2);
		cairo_stroke(cr);
		cairo_move_to(cr, width/2, 0);
		cairo_line_to(cr, width/2, height);
		cairo_stroke(cr);
	}

	// label axes
	if (session->ui.label_axes) {
		int start_x = (int)(center_x+0.5);
		int start_y = (int)(center_y+0.5);
		cairo_text_extents_t extents;

		cairo_set_font_size(cr, 12);
		for (int x = start_x-40; x>0; x-=40) {
			snprintf(buf, 128, "%.3g",
				(double)(start_x-x)/start_x*session->ui.xmin);
			cairo_text_extents(cr, buf, &extents);
			cairo_move_to(cr, x - extents.width/2, 
				height/2 + extents.height + 2);
			cairo_show_text(cr, buf);
		}
	}

	cairo_restore(cr);

	return FALSE;
}
