//
// Created by adminlocal on 12/03/2022.
//

#ifndef TESTISA_ILG_KINEIS_H
#define TESTISA_ILG_KINEIS_H

#define TURN_ON_GPS 1
#define TRAME_NMEA 2
#define PARSE_DATA 3
#define URC_REPORT 4 
#define TURN_OFF_GPS 5 
#define REQUEST_TX_DATA 6
#define MAX_POSITION_LEN 50

typedef struct {
		char latitude[MAX_POSITION_LEN];
		char longitude[MAX_POSITION_LEN];
} Position;

#endif //TESTISA_ILG_KINEIS_H

int r_gps(char *lecture);
int w_gps(int action);
int init_gps();
int r_tx_data(Position p);
void close_gps();
void get_lat(char lat_string[100], char latitude[50]);
void get_lon(char lon_string[100], char longitude[50]);
void format_position();
int get_position(Position *p);
