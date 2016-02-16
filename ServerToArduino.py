import serial
import time

#Write data to the Arduino
def write(data):
    data = chr(data)
    arduino.write(data)

if __name__ == "__main__":
    # Open connection to the serial port connected to the Arduino
    arduino = serial.Serial('COM7', 9600)

    # Give time for the Arduino to reset
    time.sleep(2)
    while 1:
        # Write data to Arduino
        data = input('Enter input: ')
        data = chr(data)
        arduino.write(data)
        print(data)






    
    
