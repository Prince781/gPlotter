//gplotter.h

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>

#define GPLOTTER_VERSION "0.1 (Alpha)"
#define GPLOTTER_MAX_SESSIONS 30

static int current_gplotter_session = 0;

//manage sessions for different windows
typedef struct {
	//graphics values
	cairo_t *cr;
	cairo_surface_t *canvas_surface;
	GdkWindow *surface_window;
	char *filename; //if a file is open, what is its filename?
	
} gPlotterSession;

gPlotterSession *gplotter_sessions[GPLOTTER_MAX_SESSIONS];

gPlotterSession *gplotter_session_new() {
	gPlotterSession *session = NULL;
	return session;
}

void gplotter_append_session(gPlotterSession *session) {
	//append at the latest index
	for (int i=0; i<GPLOTTER_MAX_SESSIONS; i++)
		if (gplotter_sessions[i] == NULL) {
			gplotter_sessions[i] = session;
			break;
		}
}

gPlotterSession *gplotter_session_next() {
	//for iteration over next session
	return gplotter_sessions[(current_gplotter_session == GPLOTTER_MAX_SESSIONS
	                          ? current_gplotter_session=0
	                          : current_gplotter_session++)];
}

gboolean gplotter_insert_session(gPlotterSession *session, int index) {
	if (index >= GPLOTTER_MAX_SESSIONS) return TRUE;
	gPlotterSession *sessions[GPLOTTER_MAX_SESSIONS-index];
	//save sessions to the right of the index
	for (int j=index+1; j<GPLOTTER_MAX_SESSIONS; j++) {
		sessions[j-(index+1)] = gplotter_sessions[j];
		gplotter_sessions[j] = NULL;
	}
	gplotter_sessions[index] = session;
	for (int j=index+1; j<GPLOTTER_MAX_SESSIONS; j++)
		gplotter_sessions[j] = sessions[j-(index+1)];
	return TRUE;
}

void gplotter_session_set_cairo_t(gPlotterSession *session, cairo_t *cr) {
	session->cr = cr;
}

void gplotter_session_set_cairo_surface(gPlotterSession *session, cairo_surface_t *surface) {
	session->canvas_surface = surface;
}

void gplotter_session_set_window(gPlotterSession *session, GdkWindow *window) {
	session->surface_window = window;
}