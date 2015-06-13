#ifndef _GPLOTTER_APP_H
#define _GPLOTTER_APP_H

#include <gtk/gtk.h>

struct _GPlotterApp {
	GtkApplication parent;

	GtkApplicationWindow *app_window;
};

#define GPLOTTER_TYPE_APP	(gplotter_app_get_type ())
G_DECLARE_FINAL_TYPE(GPlotterApp, gplotter_app, GPLOTTER, APP, GtkApplication)

GPlotterApp *gplotter_app_new(void);

#endif
