#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "gps_lib.h"

int main(void) {
    Position myPosition;

	get_position(&myPosition);

	printf("latitude: %s longitude: %s\r\n", myPosition.latitude, myPosition.longitude);
	// ex: latitude: 50.26131195 longitude: 002.49534954
	
	return 0;
}
