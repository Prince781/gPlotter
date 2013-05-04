/************************************************
 * GPlotter - version 0.1 alpha
 * A neat little program for displaying the
 * output of functions in various ways, among
 * them, as a representation along a Cartesian
 * plane in two-dimensional space.
 * ++++++++++++++++++++++++++++++++++++++++++++++
 * 2013 Princeton Ferro
*************************************************/
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <cairo.h>
#include "gplotter.h" //config data

GtkApplication *app;

static void show_about() { //display about dialogs
	GtkWindow *parent = gtk_application_get_active_window(GTK_APPLICATION(app));
	gtk_show_about_dialog(parent,
	                      "comments", _("A useful program for graphing the output of functions, and solving them."),
	                      "copyright", _("Copyright \xc2\xa9 2013 Princeton Ferro"),
	                      "program-name", _("gPlotter"),
	                      "version", GPLOTTER_VERSION_STRING,
	                      "website", _("https://github.com/Prince781/gPlotter"),
	                      "license-type", GTK_LICENSE_GPL_2_0,
	                      "website-label", _("GitHub Page"),
	                      NULL);
}

static void show_help(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
	g_print("This should show the help data...");
}

static void new_window() {
	g_print("This should create a new window.");
}

static void quit() { //quit the program
	g_print("Quitting the application now...");
	g_application_quit(G_APPLICATION(app));
}

static void new_session(GtkApplication *app, gpointer user_data) {
	//create a new window, create a new session, and initialize widgets:
	GtkWidget *window;
	GtkWidget *window_content;
	GtkWidget *window_top;
	GtkWidget *window_top_label;
	GtkWidget *window_top_separator;
	GtkWidget *window_bottom; //everything else below
	//draw the window and do callback stuff
	window = gtk_application_window_new(app);
	gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "gPlotter");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),1000,640);

	//create main window content, top content
	window_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	window_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_container_add(GTK_CONTAINER(window), window_content);
	window_top_label = gtk_label_new("Window Top area...");
	gtk_box_pack_start(GTK_BOX(window_content), window_top, TRUE, TRUE, 0);
	//TODO: add window_top code here
	//TODO: add window_top code here
	gtk_box_pack_start(GTK_BOX(window_top), window_top_label, TRUE, FALSE, 0);
	window_top_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	//add separator to window_content
	gtk_box_pack_start(GTK_BOX(window_content), window_top_separator, TRUE, TRUE, 0);

	window_bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(window_content), window_bottom, TRUE, TRUE, 0);

	gtk_widget_set_size_request(GTK_WIDGET(window_top), 1000, 60);
	gtk_widget_set_size_request(GTK_WIDGET(window_bottom), 1000, 580);
	/*
	//create a new label
	label1 = gtk_label_new("y = sin(x - 4)");
	gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);

	//create a horizontal separator
	separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add(GTK_CONTAINER(window), separator);
	*/
	
	gtk_widget_show_all(GTK_WIDGET(window)); //show all GTK widgets
}

//end of menu functions

static void startup(GtkApplication *app, gpointer user_data) {
	static const GActionEntry actions[] = { //accessed by app.{name}
		{"newwin", new_window},
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

static void activate(GtkApplication *app, gpointer user_data) {
	new_session(app, user_data); //create a new session
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