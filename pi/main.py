# import required libs
import smbus
import time
import subprocess

# connect to the i2c bus
bus = smbus.SMBus(1)

# set the address the arduino is listening to
address = 0x04

# try to read a number from the bus, return -1 if that failed
def readNumber():
        try:
                return bus.read_byte_data(address, 1)
        except:
                return -1

# until the end of time
while True:
	# delay for a second
        time.sleep(1)

	# read the code send by the arduino
        code = readNumber()
	
	# if it's code one, start playing the audio
        if (code == 2):
                subprocess.call(["aplay", "/home/pi/CLE1-8/audio/ted sfx.wav"])

