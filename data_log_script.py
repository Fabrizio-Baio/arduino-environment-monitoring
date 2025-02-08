import serial
import time
from datetime import datetime

arduino_port = '/dev/cu.usbmodem14101'
baud_rate = 9600

ser = serial.Serial(arduino_port, baud_rate)

with serial.Serial(arduino_port, baud_rate, timeout = 1) as arduino:
    with open('data_log.txt', 'a') as file:
        while True:
            time.sleep(1)
            now = datetime.now()
            current_time = now.strftime("%H:%M:%S %d/%m/%Y")
            arduino.write(current_time.encode('utf-8') + b'\n')
            if (ser.in_waiting > 0):
                data = ser.readline().decode('utf-8').strip()
                print(data)
                if (data[0] == 'M'):
                    date = current_time[9:]
                    file.write(f"{date}: {data}\n")
                    file.flush()