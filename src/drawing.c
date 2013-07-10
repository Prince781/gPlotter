//drawings.c - cairo-based drawings

#include <gdk/gdk.h>
#include <cairo.h>

static void draw_graph() {
	/* This is the main drawing sequence that populates the scene with
	 * grid lines, labels, and axes.
	 */
	//cairo_rectangle_t *bounds;
	gPlotterSession *gp_session;
	while ((gp_session = gplotter_session_next())) {
		cairo_t *cr = gplotter_session_get_canvas_context(gp_session);
		cairo_surface_t *ct = gplotter_session_get_cairo_surface(gp_session);
		if (GPLOTTER_DEBUG) {
			printf("Managed to get cairo_t, which has a size of %lu.\n", sizeof(cr));
			printf("Managed to get cairo surface, which has a size of %lu.\n", sizeof(ct));
		}
	}
}