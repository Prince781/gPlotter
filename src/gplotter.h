//gplotter.h

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>

#define GPLOTTER_VERSION "0.9 (Alpha)"
#define GPLOTTER_MAX_SESSIONS 30

#define false 0
#define true !false
#define GPLOTTER_DEBUG true

static int current_gplotter_session = 0;

//manage sessions for different windows
typedef struct {
	int instance; //a unique instance value
	int test_value;
	//graphics values
	cairo_t *cr;
	cairo_surface_t *canvas_surface;
	GdkWindow *surface_window;
	char *filename; //if a file is open, what is its filename?
	char **equations; //list of equations
} gPlotterSession;

gPlotterSession *gplotter_sessions[GPLOTTER_MAX_SESSIONS];

void gplotter_session_add_equation(gPlotterSession *session, char *equation) {
}

void gplotter_append_session(gPlotterSession *session) {
	//append at the latest index
	for (int i=0; i<GPLOTTER_MAX_SESSIONS; i++)
		if (gplotter_sessions[i] == NULL) {
			session->instance = i;
			gplotter_sessions[i] = session;
			if (GPLOTTER_DEBUG)
				printf("Appended session, where index %d originally was NULL.\n", i);
			break;
		}
}

gPlotterSession *gplotter_session_current() {
	//since we incremented with the creation of a new session...
	if (GPLOTTER_DEBUG)
		printf("Returning session at index %d.\n", current_gplotter_session);
	return gplotter_sessions[current_gplotter_session];
}

gPlotterSession *gplotter_session_from_index(int index) {
	return gplotter_sessions[index];
}

gPlotterSession *gplotter_session_next() {
	//useful for iteration in a while loop
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

int gplotter_session_set_test_value(gPlotterSession *session, int value) {
	session->test_value = value;
	return session->test_value;
}

int gplotter_session_get_test_value(gPlotterSession *session) {
	return session->test_value;
}

void gplotter_session_set_cairo_t(gPlotterSession *session, cairo_t *cr) {
	session->cr = cr;
}

cairo_t *gplotter_session_get_canvas_context(gPlotterSession *session) {
	return session->cr;
}

void gplotter_session_set_cairo_surface(gPlotterSession *session, cairo_surface_t *surface) {
	session->canvas_surface = surface;
}

cairo_surface_t *gplotter_session_get_cairo_surface(gPlotterSession *session) {
	return session->canvas_surface;
}

void gplotter_session_set_window(gPlotterSession *session, GdkWindow *window) {
	session->surface_window = window;
}

GdkWindow *gplotter_session_get_window(gPlotterSession *session) {
	return session->surface_window;
}

gPlotterSession *gplotter_session_new() {
	/* Create a new session; add to the array, and manage.
	 * Don't set the current session to the newly-created session.
	 */
	gPlotterSession *session = malloc(sizeof(gPlotterSession *));
	if (GPLOTTER_DEBUG)
		printf("Allocated %lu bytes to a new gPlotter session.\n", sizeof(gPlotterSession *));
	gplotter_append_session(session);
	return session;
}