#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dht.h"

int main() {
	Dht dht;
	if (wiringPiSetup() == -1)
		exit(1);

	while (1) {
		read_dht11_dat(&dht);
		delay(1000); 
	}

	return 0;
}
