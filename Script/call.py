from serial.tools import list_ports
import serial


pid = 0x5740
vid = 0x0483

def getport():
    device_list = list_ports.comports()
    for device_s in device_list:
        if device_s.vid == vid and device_s.pid == 0x5740:
            return device_s.device
    return "device not found"

def write_dac(device, value):
    value = 65535 - value
    strng = b"dac " + str(value).encode()
    print(strng)
    device.write(strng)


laser = serial.Serial(getport(), 115200)  # open serial port


write_dac(laser, 60000)

laser.close()
