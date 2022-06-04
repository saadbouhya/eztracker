#include <wiringSerial.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include "gps/lib/gps.h"
#include "dht/lib/dht.h"

int main() {
		Position myPosition;
		Dht dht;
		int rc;
		struct mosquitto * mosq;
		char payload[255];

		init_gps();

		mosquitto_lib_init();
		mosq = mosquitto_new("publisher-test", true, NULL);
		rc = mosquitto_connect(mosq, "mqtt.fluux.io", 1883, 10);
		if(rc != 0){
			printf("Client could not connect to broker! Error Code: %d\n", rc);
			mosquitto_destroy(mosq);
			return -1;
		}
		printf("We are now connected to the broker!\n");

		while (1) {
				read_dht11_dat(&dht);
				get_position(&myPosition);

				sprintf(payload, "latitude: %s longitude: %s humidity: %s temperature: %s",
								 myPosition.latitude,
								 myPosition.longitude,
								 dht.humidity,
								 dht.temperature);
				printf("%s\n", payload);	
				mosquitto_publish(mosq, NULL, "eztracker_saad/test_mqtt", strlen(payload), payload, 0, false);
				printf("Message sent\n");

				delay(1000); 
		}

		mosquitto_disconnect(mosq);
		mosquitto_destroy(mosq);

		mosquitto_lib_cleanup();
		printf("Cleanup done!\n");

		return 0;
}
