# grovepi_lcd_dht.py
# dht(pin,module_type), change module_type number to use other kind of dht
# module_type:
# DHT11 0
# DHT22 1
# DHT21 2
# DHT2301 3
from grovepi import *
from grove_rgb_lcd import *
dht_sensor_port = 7 # Connect the DHt sensor to port 7
while True:
    try:
        [ temp,hum ] = dht(dht_sensor_port, 0) #Get the temperature
        print "temp =", temp, "C\thumadity =", hum,"%"
        t = str(temp)
        h = str(hum)

    except (IOError,TypeError) as e:
        print "Error"
