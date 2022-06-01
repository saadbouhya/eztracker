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

    if (init_gps() == 0) {

        w_gps(TURN_ON_GPS);
        w_gps(TRAME_NMEA);
		w_gps(PARSE_DATA);

        //for (int x = 0; x < 2000; x++) { w_kineis(REQUEST_TX_DATA); sleep(60);}

        close_gps();

        return EXIT_SUCCESS;

    } else 
		return EXIT_FAILURE;
}
