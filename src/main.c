/************************************************
 * GPlotter - version 0.1 alpha
 * A neat little program for displaying the
 * output of functions in various ways, among
 * them, as a representation along a Cartesian
 * plane in two-dimensional space.
 * ++++++++++++++++++++++++++++++++++++++++++++++
 * 2013 Princeton Ferro
*************************************************/
#include <stdio.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include "gplotter.h" //config data
#include "gp_math.c"

GtkApplication *app;

static void show_about() { //display about dialogs
	GtkWindow *parent = gtk_application_get_active_window(GTK_APPLICATION(app));
	gtk_show_about_dialog(parent,
	                      "comments", _("A useful program for graphing the output of functions."),
	                      "copyright", _("Copyright \xc2\xa9 2013 Princeton Ferro"),
	                      "program-name", _("gPlotter"),
	                      "version", GPLOTTER_VERSION_STRING,
	                      "website", _("https://github.com/Prince781/gPlotter"),
	                      "license-type", GTK_LICENSE_GPL_2_0,
	                      "website-label", _("GitHub Page"),
	                      "logo-icon-name", "application-x-executable",
	                      NULL);
}

static void show_help() { //show the help documentation
	g_print("This should show the help documentation...\n");
}

static void quit() { //quit the program
	g_print("Quitting the application now...\n");
	g_application_quit(G_APPLICATION(app));
}

static void save_document_as() { //display save dialog
	GtkWindow *parent_window = gtk_application_get_active_window(GTK_APPLICATION(app));
	GtkWidget *file_dialog = gtk_file_chooser_dialog_new("Save As",
	                                                     parent_window,
	                                                     GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                     GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
	                                                     NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_dialog), TRUE);
	if (gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog));
		//TODO: have file-saving function here, based on filename: save_file(filename)
		g_free(filename);
	}
	gtk_widget_hide_on_delete(file_dialog);
}

static void new_session() {
	//create a new window, create a new session, and initialize widgets:
	GtkWidget *window;
	GtkWidget *window_content;
	GtkWidget *window_top_alignment;
		GtkWidget *window_top;
			GtkWidget *wt_save_export_buttons_container;
				GtkWidget *wt_sebc_save;
				GtkWidget *wt_sebc_export;
			GtkWidget *wt_equation_editor;
			GtkWidget *wt_menubutton;
	GtkWidget *window_top_separator;
	GtkWidget *window_bottom; //everything else below
		GtkWidget *wb_left; //shows the actual graph
		GtkWidget *wb_separator; //separates left and right visibly
		GtkWidget *wb_right_alignment;
		GtkWidget *wb_right; //contains functions
	//draw the window and do callback stuff
	window = gtk_application_window_new(app);
	gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "gPlotter");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),900,560);
	gtk_window_set_hide_titlebar_when_maximized(GTK_WINDOW(window), TRUE);
	gtk_window_set_icon_name(GTK_WINDOW(window), "application-x-executable");

	//create main window content, top content
	window_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	window_top_alignment = gtk_alignment_new(0, 1, 1, 0);
	window_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	//set minimum widget sizes
	gtk_widget_set_size_request(window_top_alignment, 700, 36);
	gtk_widget_set_size_request(window_top, 700, 30);
	gtk_widget_set_size_request(window_content, 700, 300);

	gtk_container_add(GTK_CONTAINER(window), window_content);
	gtk_box_pack_start(GTK_BOX(window_content), window_top_alignment, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window_top_alignment), window_top);
	
	//top portion of window (window_top); content
	wt_save_export_buttons_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(window_top), wt_save_export_buttons_container, FALSE, FALSE, 15);
	wt_sebc_save = gtk_button_new();
		gtk_widget_set_name(wt_sebc_save, "wt_sebc_save");
		gtk_button_set_label(GTK_BUTTON(wt_sebc_save), "Save");
		g_signal_connect(wt_sebc_save, "clicked", G_CALLBACK(save_document_as), NULL);
	wt_sebc_export = gtk_button_new();
		gtk_widget_set_name(wt_sebc_export, "wt_sebc_export");
		gtk_button_set_label(GTK_BUTTON(wt_sebc_export), "Export");
	gtk_box_pack_start(GTK_BOX(wt_save_export_buttons_container),
	                   GTK_WIDGET(wt_sebc_save), TRUE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(wt_save_export_buttons_container),
	                 GTK_WIDGET(wt_sebc_export), TRUE, FALSE, 10);
	wt_equation_editor = gtk_entry_new();
	gtk_widget_set_name(GTK_WIDGET(wt_equation_editor), "wt_equation_editor");
	
	gtk_box_pack_start(GTK_BOX(window_top), wt_equation_editor, TRUE, TRUE, 0);
	wt_menubutton = gtk_menu_button_new();

	//settings menu-button
	gtk_button_set_use_stock(GTK_BUTTON(wt_menubutton), TRUE);
	GtkWidget *wt_menubutton_image = gtk_image_new_from_icon_name("emblem-system-symbolic", GTK_ICON_SIZE_MENU);
	gtk_widget_set_name(wt_menubutton, "wt_menubutton");
	gtk_button_set_image(GTK_BUTTON(wt_menubutton), wt_menubutton_image);
	gtk_box_pack_start(GTK_BOX(window_top), wt_menubutton, FALSE, FALSE, 15);

	//settings menu
	GMenu *s_menu = g_menu_new();
	g_menu_append(s_menu, "Show Derivative", "s_menu.show_derivative");
	GMenu *sm_funcs[2];
	sm_funcs[0] = g_menu_new();
	g_menu_append(sm_funcs[0], "Show Minima/Maxima", "s_menu.show_minima_maxima");
	g_menu_append(sm_funcs[0], "Show Inflection Point Labels", "s_menu.show_inflection_points");
	g_menu_append(sm_funcs[0], "Show Intercepts", "s_menu.show_intercepts");
	sm_funcs[1] = g_menu_new();
	g_menu_append(sm_funcs[1], "Copy LaTeX Data", "s_menu.copy_latex");
	g_menu_append(sm_funcs[1], "Copy MathML Data", "s_menu.copy_mathml");
		//add submenus to settings
	for (int i=0; i<sizeof(sm_funcs)/sizeof(GMenu*); i++)
		g_menu_append_section(s_menu, NULL, G_MENU_MODEL(sm_funcs[i]));
	
	GtkWidget *settings_menu = gtk_menu_new_from_model(G_MENU_MODEL(s_menu));
	gtk_widget_set_halign(settings_menu, GTK_ALIGN_END);
	gtk_menu_button_set_popup(GTK_MENU_BUTTON(wt_menubutton), GTK_WIDGET(settings_menu));
	
	//add separator to window content
	window_top_separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_name(window_top_separator, "wt_separator");
	gtk_box_pack_start(GTK_BOX(window_content), GTK_WIDGET(window_top_separator), FALSE, FALSE, 0);

	//add bottom window content box
	window_bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_name(GTK_WIDGET(window_bottom), "window_bottom");

	//add content to the bottom of the window
	wb_left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_widget_set_name(wb_left, "wb_left");
		gtk_widget_set_size_request(wb_left, 500, 300);
	wb_separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
		gtk_widget_set_name(wb_separator, "wb_separator");
	wb_right_alignment = gtk_alignment_new(1, 0, 0, 1);
		gtk_widget_set_size_request(wb_right_alignment, 300, 300);
		gtk_widget_set_name(wb_right_alignment, "wb_right_alignment");
	wb_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_widget_set_name(wb_right, "wb_right");
		gtk_widget_set_size_request(wb_right, 300, 300);

	//add boxes to bottom window container
	gtk_box_pack_start(GTK_BOX(window_bottom), wb_left, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window_bottom), wb_separator);
	gtk_box_pack_end(GTK_BOX(window_bottom), wb_right_alignment, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(wb_right_alignment), wb_right);
	
	//add bottom container to window
	gtk_box_pack_end(GTK_BOX(window_content), GTK_WIDGET(window_bottom), TRUE, TRUE, 0);

	gtk_widget_show_all(GTK_WIDGET(window)); //show all GTK widgets
}

//end of menu functions

static void startup() {
	static const GActionEntry actions[] = { //accessed by app.{name}
		{"newsession", new_session},
		{"help", show_help},
		{"about", show_about},
		{"quit", quit}
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
	g_menu_append(menu, "New Session", "app.newsession");
	g_menu_append_section(menu, NULL, G_MENU_MODEL(about_menu));
	gtk_application_set_app_menu(app, G_MENU_MODEL(menu));
	g_object_unref(menu);
}

static void activate() {
	new_session(); //create a new window and a new session
	
	//set css styling for future elements...
	GdkDisplay *display = gdk_display_get_default();
	GdkScreen *screen = gdk_display_get_default_screen(display);
	GtkCssProvider *css_provider = gtk_css_provider_new();

	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_path(css_provider, "src/gplotter.css", 0);

	g_object_unref(css_provider);
}

int main(int argc, char **argv) {
	printf("Found a value: %f\n", find_convergence());
	int status;
	
	app = gtk_application_new("org.gtk.gPlotter",G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);
	return status;
}