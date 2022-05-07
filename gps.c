#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>


int main() {
	int serial_port;
	char w_buff[][256] = {"AT+CGNSPWR=1\r\n", "AT+CGNSSEQ=\"RMC\"\r\n","AT+CGNSINF\r\n", "AT+CGNSURC=2\r\n", "AT+CGNSTST=1\r\n"};
	char dataOutput;
	char dataOutputString[256];
	int num=1;
	int flag = 0;
	char dat,buff[100],GGA_code[3];
	char output_setup[100];
	unsigned char IsitGGAstring=0;
	unsigned char GGA_index=0;
	unsigned char is_GGA_received_completely = 0;
	unsigned int index = 0;

	if ((serial_port = serialOpen("/dev/ttyS0", 115200)) < 0) {
		fprintf(stderr, "Unable to open serial device: %s \n", strerror(errno));
		return 1;
	}

	if (wiringPiSetup() == -1) {
		fprintf(stdout, "Unable to start wiringPi: %s \n", strerror(errno));
	}

	serialPrintf(serial_port, w_buff[0]);
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			//if (dat == 'O')
			//	printf("%c", dat);


			if (dat == 'O' && flag == 0) {
				output_setup[index++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[index++] = dat;
				printf("AT+CGNSPWR=1 --> %s\n", output_setup);
				break;
			}
		}
	}

	serialPrintf(serial_port, w_buff[1]);
	index = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[index++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[index++] = dat;
				printf("AT+CGNSSEQ=RMC --> %s\n", output_setup);
				break;
			}
		}
	}


	
	serialPrintf(serial_port, w_buff[2]);
	index = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[index++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[index++] = dat;
				printf("AT+CGNSINF --> %s\n", output_setup);
				break;
			}
		}
	}

	serialPrintf(serial_port, w_buff[3]);
	index = 0;
	flag = 0;
	while(1) {
		if (serialDataAvail(serial_port)) {
			dat = serialGetchar(serial_port);

			if (dat == 'O' && flag == 0) {
				output_setup[index++] = dat;		
				flag = 1;
			} else if (flag == 1 && dat == 'K') {
				output_setup[index++] = dat;
				printf("AT+CGNSURC=2 --> %s\n", output_setup);
				break;
			}
		}
	}

	serialPrintf(serial_port, w_buff[4]);
	index = 0;
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
			printf("GGA: %s",buff);
			is_GGA_received_completely = 0;
		  }

	}

	return 0;
}
