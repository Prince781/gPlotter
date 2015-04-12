#ifndef _GPLOTTER_APP_H
#define _GPLOTTER_APP_H

#include <gtk/gtk.h>

/* type macros */
#define GPLOTTER_APP_TYPE	(gplotter_app_get_type ())
#define GPLOTTER_APP(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), GPLOTTER_APP_TYPE, GPlotterApp))

typedef struct _GPlotterApp GPlotterApp;
typedef struct _GPlotterAppClass GPlotterAppClass;

struct _GPlotterApp {
	GtkApplication parent;
};

struct _GPlotterAppClass {
	GtkApplicationClass parent_class;
};

GType gplotter_app_get_type(void);

GPlotterApp *gplotter_app_new(void);

#endif
