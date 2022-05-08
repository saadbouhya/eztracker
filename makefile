gps: script.c
	gcc gps.c -o gps -lwiringPi
temperature: temperature_humidity.c
	gcc temperature_humidity.c -o temperature -lwiringPi -lwiringPiDev
tinudht: tinudht/tinudht.c
	gcc -o tinudht -c tinudht/tinudht.c 
test_tinudht: test_tinudht.c
	gcc test_tinudht.c tinudht.o -o test_tinudht
