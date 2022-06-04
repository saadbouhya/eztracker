all: test

gps.o: gps.c gps.h
	gcc -Wall -g -c gps.c

test: test.c gps.o
	gcc -Wall -g -o $@ $^

clean:
	rm test gps.o
