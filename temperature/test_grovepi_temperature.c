#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/grovepi.h"

#define MAXTIMINGS	85
#define DHTPIN 7	

int dht11_dat[5] = { 0, 0, 0, 0, 0 };
 
void read_dht11_dat() {
	uint8_t laststate = HIGH;
	uint8_t counter	= 0;
	uint8_t j = 0, i;
	float f; 
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode(DHTPIN, 1);
	digitalWrite(DHTPIN, 0);
	pi_sleep(18);
	digitalWrite(DHTPIN, 1);
	usleep(40);
	pinMode(DHTPIN, 0);
 
	for (i = 0; i < MAXTIMINGS; i++) {
		counter = 0;
		while ( digitalRead(DHTPIN) == laststate ) {
			counter++;
			usleep(1);
			if (counter == 255) {
				break;
			}
		}

		laststate = digitalRead(DHTPIN);
 
		if (counter == 255)
			break;
 
		if ( (i >= 4) && (i % 2 == 0) ) {
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ) {
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
	} else  {
		printf( "Data not good, skip\n" );
	}
}
 
int main(void) {
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
 
	if (init() == -1)
		exit(1);
 
	while (1) {
		read_dht11_dat();
		pi_sleep(1000); 
	}
 
	return 0;
}







