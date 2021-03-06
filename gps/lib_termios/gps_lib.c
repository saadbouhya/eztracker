//
// Created by adminlocal on 12/03/2022.
//
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

#define MODEM "/dev/ttyUSB0"
#define BAUDRATE 9600 

struct termios tio;
struct termios stdio;
struct termios old_stdio;
int tty_fd, flags;
unsigned char c='D';
Position localPosition;

int get_position(Position *p) {
		if (init_gps() == 0) {
		
		        w_gps(TURN_ON_GPS);
		        w_gps(TRAME_NMEA);
				w_gps(PARSE_DATA);
				w_gps(URC_REPORT);
				w_gps(REQUEST_TX_DATA);

				format_position();	

				strcpy(p->latitude, localPosition.latitude);
				strcpy(p->longitude, localPosition.longitude);

		        close_gps();
		
		        return EXIT_SUCCESS;
		
		 } else 
				return EXIT_FAILURE;
		
}

/* Lecture sur le GPS  */
int r_gps(char *lecture) {
    int test = 0;
    int i = 0;
    //printf("Réponse de la carte kineis : \r\n");
	
    while (test == 0) {
        if (read(tty_fd, &c, 1) > 0) {
            //write(STDOUT_FILENO, &c, 1); //S'il y a un caratère lu, affichage sur la sortie standard

			if (i < 254){
				lecture[i] = c;
				i++;
		    }
				
        } else {
            test = 1;
            lecture[i]='\0';
        }
    }
    //printf("\r\n");

    return 0;
}

/* Ecriture sur le kit de développement kineis */
int w_gps(int action) {
    char ch[25];
    char reponse[256];
	Position p;

    switch(action) {
        case TURN_ON_GPS:
				//printf("On allume le GPS \r\n");
            	strcpy(ch, "AT+CGNSPWR=1\r\n");
            	write(tty_fd, &ch, 16);
            	sleep(10);
            	r_gps(reponse);
            	//printf("Reponse du kit : %s \r\n",reponse);
            	break;

		case TURN_OFF_GPS:
				//printf("On eteint le GPS \r\n");
				strcpy(ch, "AT+CGNSPWR=0\r\n");
            	write(tty_fd, &ch, 16);
            	//sleep(2);
            	//r_gps(reponse);
				break;

        case TRAME_NMEA:
				//printf("Preciser la trame NMEA RMC\r\n");
                strcpy(ch,"AT+CGNSSEQ=\"RMC\"\r\n");
                write(tty_fd,&ch,22);
                sleep(10);
                r_gps(reponse);
                //printf("Reponse du modem : %s\r\n",reponse);
                break;

        case PARSE_DATA:
                //printf("Parser les infos depuis la trame NMEA\r\n");
                strcpy(ch,"AT+CGNSINF\r\n");
                write(tty_fd,&ch,14);
                sleep(10);
                r_gps(reponse);
                //printf("Reponse du modem : %s\r\n",reponse);
                break;

        case URC_REPORT:
                //printf("URC report: chaque 2 GNSS fix\r\n");
                strcpy(ch,"AT+CGNSURC=2\r\n");
                write(tty_fd, &ch, 16);
				sleep(10);
                r_gps(reponse);
                //printf("Reponse du modem : %s\r\n",reponse);
                break;

		case REQUEST_TX_DATA:
                //printf("Envoi de data :\r\n");
                strcpy(ch,"AT+CGNSTST=1\r\n");
                write(tty_fd,&ch,strlen(ch));
				sleep(3);
                //r_gps(reponse);
				r_tx_data(p);
                //printf("Reponse du modem : %s\r\n",reponse);
                break;

        default:
            break;
    }
    return 0;
}

/* Initialisation du module GPS */
int init_gps() {

    tcgetattr(STDOUT_FILENO,&old_stdio);

    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       //make the reads non-blocking
    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           //8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    if((tty_fd = open(MODEM , O_RDWR | O_NONBLOCK)) == -1) {
        printf("Erreur à l'ouverture du port %d\r\n",errno);
        perror("gps"); //Just if you want user interface error control
        return -1;
    } else {
        printf("Ouverture du port OK \r\n");
    }

    cfsetospeed(&tio,BAUDRATE);
    cfsetispeed(&tio,BAUDRATE);            //la vitesse de transmission est déclarée au dessus
    tcsetattr(tty_fd,TCSANOW,&tio);

    return 0;
}

void close_gps() {
    w_gps(TURN_OFF_GPS);
    close(tty_fd);
    tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
}

int r_tx_data(Position p) {
	char buff[100], GGA_code[3];
	char lat_string[100], lon_string[100];
	unsigned char IsitGGAstring=0;
	unsigned char GGA_index=0;
	unsigned char is_GGA_received_completely = 0;
	char latitude[50], longitude[50];
	
    while (1) {
        if (read(tty_fd, &c, 1) > 0) {
            //write(STDOUT_FILENO, &c, 1); //S'il y a un caratère lu, affichage sur la sortie standard

			if(c == '$'){
				IsitGGAstring = 0;
				GGA_index = 0;
			} else if(IsitGGAstring ==1){
				buff[GGA_index++] = c;
				if(c == '\r')
					is_GGA_received_completely = 1;
			} else if(GGA_code[0]=='G' && GGA_code[1]=='G' && GGA_code[2]=='A'){
				IsitGGAstring = 1;
				GGA_code[0]= 0; 
				GGA_code[1]= 0;
				GGA_code[2]= 0;		
			} else {
				GGA_code[0] = GGA_code[1];
				GGA_code[1] = GGA_code[2];
				GGA_code[2] = c;
			}
		}

		if(is_GGA_received_completely==1) {
				  //printf("GGA: %s\r\n", buff);
				  //write(STDOUT_FILENO, buff, strlen(buff));
				  is_GGA_received_completely = 0;

				  strcpy(lat_string, buff);
				  strcpy(lon_string, buff);

				  get_lat(lat_string, latitude);
				  get_lon(lon_string, longitude);
				  
				  if (strlen(latitude) == 11 && strlen(longitude) == 12) {
						//printf("\r\n----- %s %s ------ \r\n", latitude, longitude);
						strcpy(localPosition.latitude, latitude);
						strcpy(localPosition.longitude, longitude);

						break; 
				  }

		}
    }
    //printf("\r\n");

    return 0;
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
