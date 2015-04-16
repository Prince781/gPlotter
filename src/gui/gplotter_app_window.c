#include "gplotter_app_window.h"
#include "grid_area.h"	/* for drawing */
#include <stdlib.h>

G_DEFINE_TYPE(GPlotterAppWindow, gplotter_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void gplotter_app_window_init(GPlotterAppWindow *win) {
	win->session = calloc(1, sizeof(struct gp_session));

	win->session->ui.colors.bg = (GdkRGBA){ 1, 1, 1, 1 };
	win->session->ui.colors.axes = (GdkRGBA){ 0.2, 0.2, 0.2, 1 };

	win->session->ui.show_axes = 1;
	win->session->ui.label_axes = 1;
	win->session->ui.xmin = win->session->ui.ymin = -10.0;
	win->session->ui.xmax = win->session->ui.ymax = 10.0;

	win->session->nfuncs = 10;
	win->session->factive = -1;
	win->session->ui_f = calloc(win->session->nfuncs, sizeof(ui_function *));
}

static void gplotter_app_window_finalize(GObject *win) {
	GPlotterAppWindow *self = GPLOTTER_APP_WINDOW(win);

	free(self->session);
	self->session = NULL;
	G_OBJECT_CLASS(gplotter_app_window_parent_class)->finalize(win);
}

static void gplotter_app_window_class_init(GPlotterAppWindowClass *class) {
	G_OBJECT_CLASS(class)->finalize = gplotter_app_window_finalize;	
}

GPlotterAppWindow *gplotter_app_window_new(GPlotterApp *app) {
	GPlotterAppWindow *gp_win;
	GtkWindow *win;

	GtkWidget *headerbar;
	GtkBuilder *hb_builder;
	GObject *hb_obj;

	GtkWidget *mw_container;
	GtkBuilder *mwc_builder;
	GObject *mwc_obj;

	GtkWidget *grid_area;
	GObject *ga_obj;
	GdkGeometry ga_hints = {
		.min_width = 400,
		.min_height = 400,
	};

	gp_win = g_object_new(GPLOTTER_APP_WINDOW_TYPE, 
		"application", app,
		"default-width", 800, "default-height", 600, 
	NULL);
	win = GTK_WINDOW(gp_win);

	// headerbar
	hb_builder = gtk_builder_new_from_resource("/org/gtk/gplotter/headerbar.ui");
	hb_obj = gtk_builder_get_object(hb_builder, "mw_headerbar");
	headerbar = GTK_WIDGET(hb_obj);
	gtk_window_set_titlebar(win, headerbar);
	gtk_widget_show(headerbar);

	// main window
	mwc_builder = gtk_builder_new_from_resource("/org/gtk/gplotter/window.ui");
	mwc_obj = gtk_builder_get_object(mwc_builder, "mw_container");
	mw_container = GTK_WIDGET(mwc_obj);
	gtk_container_add(GTK_CONTAINER(win), mw_container);
	// mw: drawing widget
	ga_obj = gtk_builder_get_object(mwc_builder, "grid_area");
	grid_area = GTK_WIDGET(ga_obj);
	gtk_window_set_geometry_hints(win, grid_area, &ga_hints,
		GDK_HINT_MIN_SIZE);
	g_signal_connect(ga_obj, "draw", G_CALLBACK(grid_draw2d), gp_win);
	gtk_widget_show(mw_container);
	
	return gp_win;
}

void gplotter_app_window_open(GPlotterAppWindow *win, GFile *file) {
}
