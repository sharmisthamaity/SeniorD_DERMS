##I2C Communication with TM4C123

import smbus
import time


BUS = 1


bus = smbus.SMBus(BUS)

##need to i2cdetect for address
ADDRESS = 0x3c

##Write byte to the register.
data = 0x05
bus.write_byte_data(ADDRESS, 0x01, data)


    


print('im done!!')


##Read two bytes from register 01, the config register.
readData1 = bus.read_byte_data(ADDRESS, 0x01)
readData2 = bus.read_word_data(ADDRESS, 0x01)
##readData2 = bus.read_byte_data(ADDRESS, 0x00)
print(readData)
print(readData2)

##Read two bytes from register 00, the ADC value.
##value = bus.read_word_data(ADDRESS,0x00) & 0xFFFF
##print('Raw ADC value: 0x{0:04X}'.format(value))
