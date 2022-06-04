#include <gtk/gtk.h>
#include <string.h>


GtkWidget *result, *window, *grid, *get_data, *test;

void do_get_data(GtkWidget *get_data, gpointer data) {
		char buffer[32] = "************************************";
		printf("fuck\n");

		gtk_label_set_text(GTK_LABEL(result), buffer);
}

int main(int argc, char* argv[]) {
		gtk_init(&argc, &argv);

		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);	
		g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

		grid = gtk_grid_new();
		gtk_container_add(GTK_CONTAINER(window), grid);

		get_data = gtk_button_new_with_label("Get Data");
		g_signal_connect(get_data, "clicked", G_CALLBACK(do_get_data), NULL);
		gtk_grid_attach(GTK_GRID(grid), get_data, 2, 0, 1, 1);

		test = gtk_label_new("   -------------->   ");
		gtk_grid_attach(GTK_GRID(grid), test, 1, 0, 1, 1);

		result = gtk_label_new("Output:");
		gtk_grid_attach(GTK_GRID(grid), result, 0, 0, 1, 1);

		gtk_widget_show_all(window);
		gtk_main();

		return 0;
}
