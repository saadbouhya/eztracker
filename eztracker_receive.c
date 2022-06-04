#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <mosquitto.h>


GtkWidget *result, *window, *grid, *get_data, *test, *stop;
struct mosquitto *mosq;

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	if(mosquitto_subscribe(mosq, NULL, "eztracker_saad/test_mqtt", 2) != MOSQ_ERR_SUCCESS){
		printf("Error with subscription \n");
		exit(-1);
	}
	else printf(" subscription ok \n");
	
	
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
	gtk_label_set_text(GTK_LABEL(result), (char *) msg->payload);
}

void do_get_data(GtkWidget *get_data, gpointer data) {
		int rc, id=123;

		mosquitto_lib_init();


		mosq = mosquitto_new("subscribe-test", true, &id);
		mosquitto_connect_callback_set(mosq, on_connect);
		mosquitto_message_callback_set(mosq, on_message);
		
		rc = mosquitto_connect(mosq, "mqtt.fluux.io", 1883, 10);
		if(rc) {
			printf("Could not connect to Broker with return code %d\n", rc);
			//return -1;
		}

		mosquitto_loop_start(mosq);
}

void stop_data(GtkWidget *stop, gpointer data) {
		char buffer[32] = "shut down!\n";
		mosquitto_loop_stop(mosq, true);

		printf("shut down");

		mosquitto_disconnect(mosq);
		mosquitto_destroy(mosq);
		mosquitto_lib_cleanup();

		gtk_label_set_text(GTK_LABEL(result), buffer);
}

int main(int argc, char* argv[]) {

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	get_data = gtk_button_new_with_label("Start");
	g_signal_connect(get_data, "clicked", G_CALLBACK(do_get_data), NULL);
	gtk_grid_attach(GTK_GRID(grid), get_data, 2, 0, 1, 1);

	stop = gtk_button_new_with_label("Stop");
	g_signal_connect(stop, "clicked", G_CALLBACK(stop_data), NULL);
	gtk_grid_attach(GTK_GRID(grid), stop, 3, 0, 5, 3);

	test = gtk_label_new("   -------------->   ");
	gtk_grid_attach(GTK_GRID(grid), test, 1, 0, 1, 1);

	result = gtk_label_new("Output:");
	gtk_grid_attach(GTK_GRID(grid), result, 0, 0, 1, 1);

	gtk_widget_show_all(window);


	gtk_main();

	return 0;
}
