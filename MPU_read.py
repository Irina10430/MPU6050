import serial

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0)
file = open('w.txt', 'w')

for i in range(100000):
    reading = ser.read(512)
    reading = reading.decode('UTF-8')
    print(reading)
    file.write(reading)

file.close()
