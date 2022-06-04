#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dht.h"


int dht11_dat[5] = { 0, 0, 0, 0, 0 };
 
void read_dht11_dat(Dht *dht) {
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHTPIN, INPUT);
	
	// 
 
	for ( i = 0; i < MAXTIMINGS; i++ ) {
		counter = 0;

		while ( digitalRead(DHTPIN) == laststate ) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = digitalRead(DHTPIN);
 
		if (counter == 255)
			break;
 
		if ( (i >= 4) && (i % 2 == 0) ) {
			dht11_dat[j / 8] <<= 1;
			//if ( counter > 16 )
			if (counter > 50)
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ) {
		f = dht11_dat[2] * 9. / 5. + 32;
		printf("Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
		dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f);

		sprintf(dht->humidity, "%d.%d", dht11_dat[0], dht11_dat[1]);
		sprintf(dht->temperature, "%d.%d", dht11_dat[2], dht11_dat[3]);
		//printf("%s ----- %s\n", dht->humidity, dht->temperature);

	} else  {
		printf("Data not good, skip\n");
	}
}
