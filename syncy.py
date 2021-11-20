
# send value to arduino and receive request from arduino to send next data repeatedly...


import serial
import time

serialcomm = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

i = 0

while True:

    i = i+1
    while True:

        # data sending to arduino
        serialcomm.write(str(i).encode())
        print(i)
        # data sent by arduino
        ardata = serialcomm.readline()
        # ardata = serialcomm.readline().decode('ascii')
        # print(ardata)
        if len(ardata) > 2:
            break
    if(i > 9):
        break
