/************************************************
 * GPlotter - version 1.0
 * A neat little program for displaying the
 * output of functions in various ways, among
 * them, as a representation along a Cartesian
 * plane in two-dimensional space.
 * ++++++++++++++++++++++++++++++++++++++++++++++
 * 2012 Princeton Ferro
*************************************************/
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <cairo.h>

const gchar *GPLOTTER_VERSION_STRING = "0.1 Alpha";
#define GPLOTTER_VERSION 0.1a

/* list of menu response functions, below, must use the format:
 * static void (GSimpleAction *action, GVariant *parameter, gpointer user_data)
**/

GtkApplication *app;
static void view_log(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	g_print("This should output the event log...");
}

static void show_about() {
	GtkWindow *parent = gtk_application_get_active_window(GTK_APPLICATION(app));
	const gchar *authors[] = {"Princeton Ferro", NULL};
	gtk_show_about_dialog(parent,
	                      "authors", authors,
	                      "comments", _("A useful program for graphing the output of functions."),
	                      "copyright", _("Copyright \xc2\xa9 2012 Princeton Ferro"),
	                      "program-name", _("gPlotter"),
	                      "version", GPLOTTER_VERSION_STRING,
	                      "website", _("https://github.com/Prince781/gPlotter"),
	                      "website-label", _("Homepage"),
	                      NULL);
}

static void show_help(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	g_print("This should show the help data...");
}

static void new_window() {
	g_print("This should create a new window.");
}

static void quit() { //quit the program
	g_application_quit(G_APPLICATION(app));
}

//end of menu functions

static void startup(GtkApplication *app, gpointer user_data) {
	static const GActionEntry actions[] = { //accessed by app.{name}
		{"newwin", new_window},
		{"viewlog", view_log },
		{"about", show_about },
		{"help", show_help },
		{"quit", quit }
	};
	GMenu *menu;
	GMenu *about_menu;
	about_menu = g_menu_new();
	g_menu_append(about_menu, "About gPlotter", "app.about");
	g_menu_append(about_menu, "Help", "app.help");
	g_menu_append(about_menu, "Quit", "app.quit");
	g_action_map_add_action_entries(G_ACTION_MAP(app), actions, G_N_ELEMENTS(actions), app);
	//add visible items (labels) to the menu
	menu = g_menu_new();
	g_menu_append(menu, "New Window", "app.newwin");
	g_menu_append(menu,"View Log","app.viewlog");
	g_menu_append_section(menu, NULL, G_MENU_MODEL(about_menu));
	gtk_application_set_app_menu(app, G_MENU_MODEL(menu));
	g_object_unref(menu);
}

static void activate(GtkApplication *app, gpointer user_data) { //show the window
	GtkWidget *window;
	//draw the window and do callback stuff
	window = gtk_application_window_new(app);
	gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "gPlotter");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),1000,640);
	gtk_widget_show_all(GTK_WIDGET(window));
}

int main(int argc, char **argv) {
	int status;
	
	app = gtk_application_new("org.gtk.gplotter",G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);
	return status;
}