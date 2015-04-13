#include "gplotter_app_window.h"

G_DEFINE_TYPE(GPlotterAppWindow, gplotter_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void gplotter_app_window_init(GPlotterAppWindow *win) { }

static void gplotter_app_window_class_init(GPlotterAppWindowClass *class) { }

GPlotterAppWindow *gplotter_app_window_new(GPlotterApp *app) {
	GPlotterAppWindow *gp_win;
	GtkWindow *win;

	GtkWidget *headerbar;
	GtkBuilder *hb_builder;
	GObject *hb_obj;

	GtkWidget *mw_container;
	GtkBuilder *mwc_builder;
	GObject *mwc_obj;	

	gp_win = g_object_new(GPLOTTER_APP_WINDOW_TYPE, 
		"application", app, 
		"default-height", 600, "default-width", 800,
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
	gtk_widget_show(mw_container);
	
	return gp_win;
}

void gplotter_app_window_open(GPlotterAppWindow *win, GFile *file) {
}
