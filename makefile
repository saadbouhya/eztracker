gps: script.c
	gcc gps.c -o gps -lwiringPi
temperature: temperature_humidity.c
	gcc temperature_humidity.c -o temperature -lwiringPi -lwiringPiDev
