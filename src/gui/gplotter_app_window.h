#ifndef _GPLOTTER_APP_WINDOW_H
#define _GPLOTTER_APP_WINDOW_H

#include <gtk/gtk.h>
#include "gplotter_app.h"

/* type macros */
#define GPLOTTER_APP_WINDOW_TYPE	(gplotter_app_window_get_type ())
#define GPLOTTER_APP_WINDOW(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	 GPLOTTER_APP_WINDOW_TYPE, GPlotterAppWindow))

typedef struct _GPlotterAppWindow GPlotterAppWindow;
typedef struct _GPlotterAppWindowClass GPlotterAppWindowClass;

struct _GPlotterAppWindow {
	GtkApplicationWindow parent;
};

struct _GPlotterAppWindowClass {
	GtkApplicationWindowClass parent_class;
};

GType gplotter_app_window_get_type(void);

GPlotterAppWindow *gplotter_app_window_new(GPlotterApp *app);

void gplotter_app_window_open(GPlotterAppWindow *win, GFile *file);

#endif
