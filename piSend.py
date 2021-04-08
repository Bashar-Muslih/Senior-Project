#I don't know Python.
#https://pyserial.readthedocs.io/en/latest/pyserial_api.html#classes

#!/usr/bin/env python3
import serial
import time

x = 20
y = -30

if __name__ == '__main__':
  ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #Initializes port. Increase timeout value if not reading fully.
  #Serial(port, baudrate, bytesize, parity, stopbits, timeout, xonxoff, rtscts, write_timeout, dsrdtr, inter_byte_timeout, exclusive)
  #The port is immediately opened on object creation, when a port is given. It is not opened when port is None and a successive call to open() is required.
  #port is a device name: depending on operating system. e.g. /dev/ttyUSB0 on GNU/Linux or COM3 on Windows.
  ser.flush() #Wait until all data is written.
  
  ser.write(str(x)"\n".encode('ascii')) #Write the bytes to the port. This should be of type bytes.
  ser.write(str(y)"\n)".encode('ascii')) #Write the bytes to the port. This should be of type bytes.
  
  #Print to see if values are correct
  value2 = ser.read_until() #
  value1 = ser.read_until()
  print("x = ", value1, "\n")
  print("y = ", value2, "\n")
