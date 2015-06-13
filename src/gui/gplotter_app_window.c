#include "gplotter_app_window.h"
#include "grid_area.h"	/* for drawing */
#include <stdlib.h>

G_DEFINE_TYPE(GPlotterAppWindow, gplotter_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void gplotter_app_window_init(GPlotterAppWindow *win) {
	win->session = g_new0(struct gp_session, 1);

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

	g_free(self->session);
	self->session = NULL;
	G_OBJECT_CLASS(gplotter_app_window_parent_class)->finalize(win);
}

static void gplotter_app_window_class_init(GPlotterAppWindowClass *class) {
	G_OBJECT_CLASS(class)->finalize = gplotter_app_window_finalize;	
}

GPlotterAppWindow *gplotter_app_window_new(GPlotterApp *app) {
	GPlotterAppWindow *gp_win = g_object_new(GPLOTTER_TYPE_APP_WINDOW,
		"application", app,
		"default-width", 800,
		"default-height", 600,
		NULL);

	// headerbar
	g_autoptr(GtkBuilder) hb_builder = gtk_builder_new_from_resource("/org/gtk/gplotter/headerbar.ui");
	GtkWidget *headerbar = GTK_WIDGET(gtk_builder_get_object(hb_builder, "mw_headerbar"));
	gtk_window_set_titlebar(GTK_WINDOW(gp_win), headerbar);

	// main window
	g_autoptr(GtkBuilder) mwc_builder = gtk_builder_new_from_resource("/org/gtk/gplotter/window.ui");
	GtkWidget *mw_container = GTK_WIDGET(gtk_builder_get_object(mwc_builder, "mw_container"));
	gtk_container_add(GTK_CONTAINER(gp_win), mw_container);

	// mw: drawing widget
	GtkWidget *grid_area = GTK_WIDGET(gtk_builder_get_object(mwc_builder, "grid_area"));

	GdkGeometry ga_hints = { .min_width = 400, .min_height = 400 };
	gtk_window_set_geometry_hints(GTK_WINDOW(gp_win), grid_area, &ga_hints, GDK_HINT_MIN_SIZE);

	g_signal_connect(G_OBJECT(grid_area), "draw", G_CALLBACK(grid_draw2d), gp_win);

	app->app_window = GTK_APPLICATION_WINDOW(gp_win);

	gtk_widget_show_all(GTK_WIDGET(gp_win));
	return gp_win;
}

void gplotter_app_window_open(GPlotterAppWindow *win, GFile *file) {
}
