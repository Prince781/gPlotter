#include <stdio.h>
#include "grid_area.h"
#include "gplotter_app_window.h"

#define gdk_rgba_grayscale(color) (GdkRGBA){\
	.red = 0.21*color.red + 0.72*color.green + 0.07*color.blue,\
	.green = 0.21*color.red + 0.72*color.green + 0.07*color.blue,\
	.blue = 0.21*color.red + 0.72*color.green + 0.07*color.blue,\
	.alpha = color.alpha\
}

gboolean grid_draw2d(GtkWidget *widget, cairo_t *cr, gpointer data) {
	unsigned width, height;
	struct gp_session *session;
	char buf[128];
	double center_x, center_y;
	int start_x, start_y;
	double diff_x, diff_y;
	GdkRGBA grid_color;
	
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);
	session = GPLOTTER_APP_WINDOW(data)->session;
	diff_x = session->ui.xmax - session->ui.xmin;
	diff_y = session->ui.ymax - session->ui.ymin;
	grid_color = gdk_rgba_grayscale(session->ui.colors.axes);
	grid_color.alpha *= 0.4;

	center_x = -(session->ui.xmin / diff_x) * width;
	center_y = -(session->ui.ymin / diff_y) * height;
	start_x = (int)(center_x+0.5);
	start_y = (int)(center_y+0.5);
	cairo_save(cr);

	// fill
	gdk_cairo_set_source_rgba(cr, &session->ui.colors.bg);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_fill(cr);

	// draw axes
	if (session->ui.show_axes) {
		gdk_cairo_set_source_rgba(cr, &session->ui.colors.axes);
		cairo_set_line_width(cr, 1);
		cairo_move_to(cr, 0, center_y);
		cairo_line_to(cr, width, center_y);
		cairo_stroke(cr);		
		cairo_move_to(cr, center_x, 0);
		cairo_line_to(cr, center_x, height);
		cairo_stroke(cr);

		cairo_save(cr);
		gdk_cairo_set_source_rgba(cr, &grid_color);
		for (int x = start_x-40; x>0; x-=40) {
			cairo_move_to(cr, x, 0);
			cairo_line_to(cr, x, height);
			cairo_stroke(cr);
		}
		for (int x = start_x+40; x<width; x+=40) {
			cairo_move_to(cr, x, 0);
			cairo_line_to(cr, x, height);
			cairo_stroke(cr);
		}
		for (int y = start_y-40; y>0; y-=40) {
			cairo_move_to(cr, 0, y);
			cairo_line_to(cr, width, y);
			cairo_stroke(cr);
		}
		for (int y = start_y+40; y<height; y+=40) {
			cairo_move_to(cr, 0, y);
			cairo_line_to(cr, width, y);
			cairo_stroke(cr);
		}
		cairo_restore(cr);
	}

	// label axes
	if (session->ui.label_axes) {
		cairo_text_extents_t extents;

		cairo_set_font_size(cr, 10);
		for (int x = start_x-40; x>0; x-=40) {
			snprintf(buf, 128, "%.3g",
				(double)(start_x-x)/start_x*session->ui.xmin);
			cairo_text_extents(cr, buf, &extents);
			cairo_move_to(cr, x - extents.width/2, 
				center_y + extents.height + 2);
			cairo_show_text(cr, buf);
		}

		for (int x = start_x; x<width; x+=40) {
			snprintf(buf, 128, "%.3g",
				(double)(x-start_x)/(width-start_x)*session->ui.xmax);
			cairo_text_extents(cr, buf, &extents);
			cairo_move_to(cr, x - extents.width/2, 
				center_y + extents.height + 2);
			cairo_show_text(cr, buf);
		}

		for (int y = start_y-40; y>0; y-=40) {
			snprintf(buf, 128, "%.3g",
				-(double)(start_y-y)/start_y*session->ui.ymin);
			cairo_text_extents(cr, buf, &extents);
			cairo_move_to(cr, center_x - extents.width - 2, 
				y + extents.height/2);
			cairo_show_text(cr, buf);
		}

		for (int y = start_y+40; y<height; y+=40) {
			snprintf(buf, 128, "%.3g",
				-(double)(y-start_y)/(height-start_y)*session->ui.ymax);
			cairo_text_extents(cr, buf, &extents);
			cairo_move_to(cr, center_x - extents.width - 2, 
				y + extents.height/2);
			cairo_show_text(cr, buf);
		}
	}

	// draw functions
	if (session->factive >= 0 && session->factive < session->nfuncs
	&& session->ui_f[session->factive] != NULL) {
		ui_function *uif = session->ui_f[session->factive];
		double x, y, z;
		int xp, yp;
		int path_started = 0;

		gdk_cairo_set_source_rgba(cr, uif->color);
		if (uif->func->nvars == 1) {		// 2D
			for (xp=0; xp<=width; ++xp) {
				x = session->ui.xmin + diff_x*(double)xp/width;
				y = function_veval(uif->func, x);
				yp = (int)(height * 
					(y-session->ui.ymin)/diff_y + 0.5);
				if (!path_started) {
					cairo_move_to(cr, xp, yp);
					path_started = 1;
				} else
					cairo_line_to(cr, xp, yp);
			}
			cairo_stroke(cr);
		}
	}

	cairo_restore(cr);

	return FALSE;
}
