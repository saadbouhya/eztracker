#include <wiringSerial.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "gps.h"

int serial_port;
char dat;
int flag = 0;
char output_setup[100];
unsigned int i = 0;
Position localPosition;
char w_buff[][256] = {"AT+CGNSPWR=1\r\n", 
					  "AT+CGNSSEQ=\"RMC\"\r\n",
					  "AT+CGNSINF\r\n", 
					  "AT+CGNSURC=2\r\n", 
					  "AT+CGNSTST=1\r\n",
				      "AT+CGNSPWR=0\r\n"};

int init_gps() {

    if ((serial_port = serialOpen(MODEM, BAUDRATE)) < 0) {
		fprintf(stderr, "Unable to open serial device: %s \n", strerror(errno));
		return -1;
	}

	if (wiringPiSetup() == -1) {
		fprintf(stdout, "Unable to start wiringPi: %s \n", strerror(errno));
		return -1;
	}

	serialPrintf(serial_port, w_buff[0]);
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[i++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[i++] = dat;
				printf("AT+CGNSPWR=1 --> %s\n", output_setup);
				break;
			}
		}
	}

	serialPrintf(serial_port, w_buff[1]);
	i = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[i++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[i++] = dat;
				printf("AT+CGNSSEQ=RMC --> %s\n", output_setup);
				break;
			}
		}
	}

	serialPrintf(serial_port, w_buff[2]);
	i = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[i++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[i++] = dat;
				printf("AT+CGNSINF --> %s\n", output_setup);
				break;
			}
		}
	}

	serialPrintf(serial_port, w_buff[3]);
	i = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[i++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[i++] = dat;
				printf("AT+CGNSURC=2 --> %s\n", output_setup);
				break;
			}
		}
	}

	return 1;
}

void close_gps() {
    serialPrintf(serial_port, w_buff[5]);
	i = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[i++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[i++] = dat;
				printf("AT+CGNSPWR=0 --> %s\n", output_setup);

				break;
			}
		}
	}
}

void get_position(Position *p) {
 	unsigned char IsitGGAstring=0;
	unsigned char GGA_index=0;
	unsigned char is_GGA_received_completely = 0;
    char buff[100],GGA_code[3];
    char lat_string[100], lon_string[100];
	char latitude[50], longitude[50];

    serialPrintf(serial_port, w_buff[4]);
	i = 0;
	flag = 0;
	while(1) {
		 if(serialDataAvail (serial_port)) {
			dat = serialGetchar(serial_port);
			if(dat == '$'){
				IsitGGAstring = 0;
				GGA_index = 0;
			} else if(IsitGGAstring ==1){
				buff[GGA_index++] = dat;
				if(dat=='\r')
					is_GGA_received_completely = 1;
			} else if(GGA_code[0]=='G' && GGA_code[1]=='G' && GGA_code[2]=='A'){
				IsitGGAstring = 1;
				GGA_code[0]= 0; 
				GGA_code[1]= 0;
				GGA_code[2]= 0;		
			} else {
				GGA_code[0] = GGA_code[1];
				GGA_code[1] = GGA_code[2];
				GGA_code[2] = dat;
			}
		  }

		  if(is_GGA_received_completely==1) {
			//printf("GGA: %s\r\n",buff);
			is_GGA_received_completely = 0;

			strcpy(lat_string, buff);
			strcpy(lon_string, buff);

			get_lat(lat_string, latitude);
			get_lon(lon_string, longitude);
				  
			if (strlen(latitude) == 11 && strlen(longitude) == 12) {
					strcpy(localPosition.latitude, latitude);
					strcpy(localPosition.longitude, longitude);

					format_position();

					strcpy(p->latitude, localPosition.latitude);
					strcpy(p->longitude, localPosition.longitude);
				    printf("GGA: %s\r\n",buff);

					break; 
			}
		  }
	}
}

void get_lat(char lat_string[100], char latitude[50]) {
		char *token = strtok(lat_string, ",");
		int index_data = 0;
		
		while(token != NULL) {
		  	  token = strtok(NULL, ",");
		  	  index_data++;
		
		  	  if (index_data == 1) {
					  strcpy(latitude, token);
		  	  }
		}

}

void get_lon(char lon_string[100], char longitude[50]) {
		char *token = strtok(lon_string, ",");
		int index_data = 0;
		
		while(token != NULL) {
		  	  token = strtok(NULL, ",");
		  	  index_data++;
		
		  	  if (index_data == 3) {
					  strcpy(longitude, token);
		  	  }
		}
}

void format_position() {
		char dot = '.';

		localPosition.latitude[4] = localPosition.latitude[3];
		localPosition.latitude[3] = localPosition.latitude[2];
		localPosition.latitude[2] = dot;

		localPosition.longitude[5] = localPosition.longitude[4];
		localPosition.longitude[4] = localPosition.longitude[3];
		localPosition.longitude[3] = dot;
}
