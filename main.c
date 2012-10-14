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
#include <cairo.h>

/* list of menu functions, below, must use the format:
 * static void (GSimpleAction *action, GVariant *parameter, gpointer user_data)
**/

static void new_window(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	g_print("This should create a new window...");
}

static void show_about(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	g_print("This should show the about dialog...");
}

static void show_help(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	g_print("This should show the help data...");
}

static void quit(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	GtkApplication *app = user_data;
	g_application_quit(app); //quit the program
}

//end of menu functions

static void startup(GtkApplication *app, gpointer user_data) {
	static const GActionEntry actions[] = { //accessed by app.{name}
		{"new", new_window },
		{"about", show_about },
		{"help", show_help },
		{"quit", quit }
	};
	GMenu *menu;
	g_action_map_add_action_entries(G_ACTION_MAP(app), actions, G_N_ELEMENTS(actions), app);
	//add visible items (labels) to the menu
	menu = g_menu_new();
	g_menu_append(menu,"New Window","app.new");
	//g_menu_append_section (menu, "Testing",);
	g_menu_append(menu, "About gPlotter", "app.about");
	g_menu_append(menu, "Help", "app.help");
	g_menu_append(menu, "Quit", "app.quit");
	gtk_application_set_app_menu(app, G_MENU_MODEL(menu));
	g_object_unref(menu);
}

static void activate(GtkApplication *app, gpointer user_data) {
	//draw the window and do callback stuff
	GtkWidget *window;
	window = gtk_application_window_new(app);
	gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "gPlotter");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),800,500);
	gtk_widget_show_all(GTK_WIDGET(window));
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	
	app = gtk_application_new("org.gtk.gplotter",G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);
	return status;
}