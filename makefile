all: eztracker_send eztracker_receive

eztracker_receive: eztracker_receive.c 
	gcc -Wall -g `pkg-config --cflags gtk+-3.0` -o $@ $^ -lmosquitto `pkg-config --libs gtk+-3.0`

eztracker_send: eztracker_send.c gps/lib/gps.o dht/lib/dht.o 
	gcc -Wall -g  -o $@ $^ -lwiringPi -lmosquitto 

clean:
	rm eztracker_send eztracker_receive
