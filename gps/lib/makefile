all: test

gps.o: gps.c gps.h
	gcc -Wall -g -c gps.c -lwiringPi

test: test.c gps.o
	gcc -Wall -g -o $@ $^ -lwiringPi 

clean:
	rm test gps.o
