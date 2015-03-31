#ifndef _GPLOTTER_APP_H
#define _GPLOTTER_APP_H
#include <gtk/gtk.h>

struct _GPlotterApp {
	GtkApplication parent_instance;
};

struct _GPlotterAppClass {
	GtkApplicationClass parent;
};

typedef struct _GPlotterApp GPlotterApp;
typedef struct _GPlotterAppClass GPlotterAppClass;

#endif
