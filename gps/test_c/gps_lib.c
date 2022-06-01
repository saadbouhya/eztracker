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
#define BAUDRATE 115200 

struct termios tio;
struct termios stdio;
struct termios old_stdio;
int tty_fd, flags;
unsigned char c='D';

/* Lecture sur le GPS  */
int r_gps(char *lecture) {
    int test = 0;
    int i=0;
    //printf("Réponse de la carte kineis : \r\n");
	
    while (test == 0) {
        if (read(tty_fd, &c, 1) > 0) {
            write(STDOUT_FILENO, &c, 1); //S'il y a un caratère lu, affichage sur la sortie standard
            lecture[i]=c;
            i++;
        } else {
            test = 1;
            lecture[i]='\0';
        }
    }
    printf("\r\n");
    return 0;
}

/* Ecriture sur le kit de développement kineis */
int w_gps(int action) {
    char ch[25];
    int j=0;
    char reponse[100];

    switch(action) {
        case TURN_ON_GPS:
            printf("On allume le GPS \r\n");
            strcpy(ch, "AT+CGNSPWR=1\r\n");
            write(tty_fd, &ch, 16);
            sleep(2);
            r_gps(reponse);
            //printf("Reponse du kit : %s \r\n",reponse);
            break;
        //case REQUEST_SET_POWER:

        //        printf("Envoi de la puissance vers le modem\r\n");
        //        strcpy(ch,"AT+PWR=1000\r\n");
        //        write(tty_fd,&ch,15);
        //        sleep(2);
        //        r_kineis(reponse);
        //        printf("Reponse du modem : %s\r\n",reponse);
        //        break;
        //case REQUEST_SET_BAND:

        //        printf("Envoi de la bande vers le modem\r\n");
        //        strcpy(ch,"AT+BAND=B1\r\n");
        //        write(tty_fd,&ch,15);
        //        sleep(2);
        //        r_kineis(reponse);
        //        printf("Reponse du modem : %s\r\n",reponse);
        //        break;
        //case REQUEST_TX_DATA:
        //        printf("Envoi de data : ABC\r\n");
        //        strcpy(ch,"AT+TX=454647\r\n");
        //        write(tty_fd,&ch,strlen(ch));
        //        r_kineis(reponse);
        //        printf("Reponse du modem : %s\r\n",reponse);
        //        break;
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
    //if((tty_fd = open(MODEM , O_RDWR )) == -1){ /* Ouverture en lecture bloquante : il faudrait gérer un timeout avec un signal sur ALARM */
	
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

int close_gps() {
    close(tty_fd);
    tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
}
