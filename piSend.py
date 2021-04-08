#I don't know Python.
#https://pyserial.readthedocs.io/en/latest/pyserial_api.html#classes

#!/usr/bin/env python3
import serial
import time

if __name__ == '__main__':
  ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #Initializes port
  #Serial(port, baudrate, bytesize, parity, stopbits, timeout, xonxoff, rtscts, write_timeout, dsrdtr, inter_byte_timeout, exclusive)
  #The port is immediately opened on object creation, when a port is given. It is not opened when port is None and a successive call to open() is required.
  #port is a device name: depending on operating system. e.g. /dev/ttyUSB0 on GNU/Linux or COM3 on Windows.
  ser.flush() #Wait until all data is written.
  
  #Put float numbers x and y into parentheses of .write()
  ser.write(11) #Write the bytes to the port. This should be of type bytes.
  x = ser.readline()
  ser.write(-22) #Write the bytes to the port. This should be of type bytes.
  y = ser.readline()
  print("x = ", x, "\n")
  print("y = ", y, "\n")
  
  ser.write(-66) #Write the bytes to the port. This should be of type bytes.
  ser.write(77) #Write the bytes to the port. This should be of type bytes.
  x = ser.readline()
  y = ser.readline()
  print("x = ", x, "\n")
  print("y = ", y, "\n")
