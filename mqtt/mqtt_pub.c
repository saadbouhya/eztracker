#include <stdio.h>
#include <string.h>
#include <mosquitto.h>

int main(){
	int rc;
	struct mosquitto * mosq;
	char payload[255] = "latitude: 1234.1234345 longitude: 12345.12345 humidity: 12.1 temperature: 12.1";

	mosquitto_lib_init();

	mosq = mosquitto_new("publisher-test", true, NULL);

	rc = mosquitto_connect(mosq, "mqtt.fluux.io", 1883, 10);
	if(rc != 0){
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1;
	}
	printf("We are now connected to the broker!\n");

	mosquitto_publish(mosq, NULL, "eztracker_saad/test_mqtt", strlen(payload), payload, 0, false);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
	return 0;
}
