// window.c - implements GUI for gPlotter
#include "window.h"
#include "gplotter.h"

GtkApplication *app;
int gtkapp_status;

/* gtk-application functions */
void gtkapp_newsession() { }

void gtkapp_show_about() {
	char version[5];
	sprintf(version, "v%d.%d", GPLOTTER_MAJOR, GPLOTTER_MINOR);
	
	GtkWindow *parent = gtk_application_get_active_window(GTK_APPLICATION(app));
	gtk_show_about_dialog(parent,
		"comments", _("A useful program for graphing the output of functions."),
		"copyright", _("Copyright \xc2\xa9 2014 Princeton Ferro"),
		"program-name", _("gPlotter"),
		"version", version,
		"website", _("https://github.com/Prince781/gPlotter"),
		"license-type", GTK_LICENSE_GPL_2_0,
		"website-label", _("GitHub Page"),
		"logo-icon-name", "application-x-executable",
		NULL);
}

void gtkapp_quit() {
	printf("Quitting gPlotter.\n");
	g_application_quit(G_APPLICATION(app));
}

void gtkapp_startup() {
	static const GActionEntry actions[] = {
		{"newsession", gtkapp_newsession},
		{"about", gtkapp_show_about},
		{"quit", gtkapp_quit}
	};
	
	GMenu *menu;
	GMenu *about_menu = g_menu_new();
	g_menu_append(about_menu, "About gPlotter", "app.about");
	g_menu_append(about_menu, "Quit", "app.quit");
	g_action_map_add_action_entries(G_ACTION_MAP(app), actions, 
		G_N_ELEMENTS(actions), app);
	
	// add menu
	menu = g_menu_new();
	g_menu_append(menu, "New Session", "app.newsession");
	g_menu_append_section(menu, NULL, G_MENU_MODEL(about_menu));
	gtk_application_set_app_menu(app, G_MENU_MODEL(menu));
	g_object_unref(menu);
}

void gtkapp_activate() {
	// GdkDisplay *display = gdk_display_get_default();
	// GdkScreen *screen = gdk_display_get_default_screen(display);
	// TODO: *potentially* add CSS loading here
	
	GtkWidget *window = gtk_application_window_new(app);
	
	gtk_widget_show_all(GTK_WIDGET(window));
}

int *gtkapp_initialize(int argc, char **argv) {
	app = gtk_application_new("org.gtk.gPlotter", G_APPLICATION_FLAGS_NONE);
	
	g_signal_connect(app, "startup", G_CALLBACK(gtkapp_startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(gtkapp_activate), NULL);
	gtkapp_status = g_application_run(G_APPLICATION(app), argc, argv);
	
	g_object_unref(app);
	
	return &gtkapp_status;
}