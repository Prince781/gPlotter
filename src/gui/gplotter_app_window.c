#include "gplotter_app_window.h"

G_DEFINE_TYPE(GPlotterAppWindow, gplotter_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void gplotter_app_window_init(GPlotterAppWindow *win) { }

static void gplotter_app_window_class_init(GPlotterAppWindowClass *class) { }

GPlotterAppWindow *gplotter_app_window_new(GPlotterApp *app) {
	GPlotterAppWindow *gp_win;
	GtkWindow *win;
	GtkWidget *headerbar;

	gp_win = g_object_new(GPLOTTER_APP_WINDOW_TYPE, 
		"application", app, 
		"default-height", 600, "default-width", 800,
	NULL);
	win = GTK_WINDOW(gp_win);

	// headerbar
	headerbar = gtk_header_bar_new();
	gtk_window_set_titlebar(win, headerbar);
	gtk_header_bar_set_title(GTK_HEADER_BAR(headerbar), "gPlotter");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
	gtk_widget_show(headerbar);
	
	return gp_win;
}

void gplotter_app_window_open(GPlotterAppWindow *win, GFile *file) {
}
