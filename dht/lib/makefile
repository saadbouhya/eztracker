all: test

dht.o: dht.c dht.h
	gcc -Wall -g -c dht.c -lwiringPi

test: test.c dht.o
	gcc -Wall -g -o $@ $^ -lwiringPi 

clean:
	rm test dht.o
