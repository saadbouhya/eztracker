all: test_gps

#gps: script.c
#	gcc gps.c -o gps -lwiringPi

gps_lib.o: gps_lib.c gps_lib.h
	gcc -Wall -g -c gps_lib.c

test_gps: test_gps.c gps_lib.o
	gcc -Wall -g -o $@ $^
