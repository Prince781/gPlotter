// window.h - draws the main GUI for the program

#include <stdio.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <gdk/gdk.h>
#include <cairo.h>

extern GtkApplication *app; // our GTK application

extern int gtkapp_status; // the application's status

/**
 * Creates a new session in gPlotter.
 */
extern void gtkapp_newsession();

/**
 * Display the program's "about" dialog.
 */
extern void gtkapp_show_about();

/**
 * Quits the GTK application.
 */
extern void gtkapp_quit();

/**
 * Called upon the GTK program's initialization.
 */
extern void gtkapp_startup();

/**
 * Called after the window's activation. This initializes the display, sets 
 * rendering calls for elements, and organizes aspects of the visual system.
 */
extern void gtkapp_activate();

/**
 * Initializes the GUI.
 */
extern int *gtkapp_initialize(int argc, char **argv);