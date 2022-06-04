#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "gps.h"

int main() {
	Position myPosition;

	init_gps();

	get_position(&myPosition);

	sleep(1);

	printf("------------- latitude: %s longitude: %s ----------- \r\n", myPosition.latitude, myPosition.longitude);

	close_gps();

	return 0;
}
