#I don't know Python.
#https://pyserial.readthedocs.io/en/latest/pyserial_api.html#classes
#Code for testing/debugging is commented

#!/usr/bin/env python3
import serial
import time

x = +20000
y = -300000

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)
    ser.flush()
    time.sleep(1)

    ser.write("\n".encode('ascii'))
    time.sleep(1)
    ser.write(str(x).encode('ascii') + b"\n")
    #print("Sent X = " + str(x))
    time.sleep(1)   
    ser.write(str(y).encode('ascii'))
    #print("Sent Y = " + str(y)) 
    time.sleep(1)

    #while True:
        #if ser.in_waiting > 0:
            #line = ser.readline().decode('ascii').rstrip()
            #print(line)
            #time.sleep(1)
