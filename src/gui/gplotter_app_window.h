#ifndef _GPLOTTER_APP_WINDOW_H
#define _GPLOTTER_APP_WINDOW_H

#include <gtk/gtk.h>
#include "gplotter_app.h"
#include "session.h"

struct _GPlotterAppWindow {
	GtkApplicationWindow parent;

	/* instance members */
	struct gp_session *session;
};

#define GPLOTTER_TYPE_APP_WINDOW	(gplotter_app_window_get_type ())
G_DECLARE_FINAL_TYPE(GPlotterAppWindow, gplotter_app_window, GPLOTTER, APP_WINDOW, GtkApplicationWindow)

GPlotterAppWindow *gplotter_app_window_new(GPlotterApp *app);

void gplotter_app_window_open(GPlotterAppWindow *win, GFile *file);

#endif
