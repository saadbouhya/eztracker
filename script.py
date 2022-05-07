#!/usr/bin/python
import serial
import time

serial_port = serial.Serial("/dev/ttyS0",115200)

w_buff = ["AT+CGNSPWR=1\r\n", "AT+CGNSSEQ=\"RMC\"\r\n", "AT+CGNSINF\r\n", "AT+CGNSINF\r\n", "AT+CGNSURC=2\r\n", "AT+CGNSTST=1\r\n"]

serial_port.write(w_buff[0])
serial_port.flush()

data = ""
num = 0

try:
    while True:
        while serial_port.inWaiting() > 0:
            data += serial_port.read(serial_port.inWaiting())
        if data != "":
            print data
except KeyboardInterrupt:
    if serial_port != None:
        serial_port.close()
