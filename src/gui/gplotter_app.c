#include "gplotter_app.h"
#include "gplotter_app_window.h"

G_DEFINE_TYPE(GPlotterApp, gplotter_app, GTK_TYPE_APPLICATION);

static void gplotter_app_init(GPlotterApp *app) { }

static void gplotter_app_activate(GApplication *app) {
	GPlotterAppWindow *win = gplotter_app_window_new(GPLOTTER_APP(app));

	gtk_window_present(GTK_WINDOW(win));
}

static void gplotter_app_open(GApplication  *app,
                              GFile        **files,
                              gint           n_files,
                              const gchar   *hint) {
}

static void gplotter_app_shutdown(GApplication *app) {
	g_object_unref(GPLOTTER_APP(app)->app_window);
}

static gboolean gplotter_app_local_cmdline(GApplication   *app,
                                           gchar        ***args,
                                           int            *exit_status) {
	/* TODO: handle command line arguments */
	return FALSE;
}

static void gplotter_app_class_init(GPlotterAppClass *class) {
	G_APPLICATION_CLASS(class)->activate = gplotter_app_activate;
	G_APPLICATION_CLASS(class)->open = gplotter_app_open;
}

GPlotterApp *gplotter_app_new(void) {
	return g_object_new(GPLOTTER_TYPE_APP,
			    "application-id", "org.gtk.gplotterapp",
			    "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
