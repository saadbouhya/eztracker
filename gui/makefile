all: test

test: test.c
	gcc `pkg-config --cflags gtk+-3.0` -o test test.c `pkg-config --libs gtk+-3.0`

