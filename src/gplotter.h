//gplotter.h
#define GPLOTTER_VERSION 0.1
const gchar *GPLOTTER_VERSION_STRING = "0.1 (Alpha)";
const int numWindows = 1;

const typedef struct {
	int width;
	int height;
} GPLOTTER_WINDOW_GEOMETRY;

const typedef struct {
	GPLOTTER_WINDOW_GEOMETRY window;
	
} GPLOTTER_SESSION;