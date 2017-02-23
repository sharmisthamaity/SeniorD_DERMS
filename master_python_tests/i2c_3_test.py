##I2C Communication with TM4C123

import smbus
import time

BUS = 1

bus = smbus.SMBus(BUS)

##need to i2cdetect for address
ADDRESS = 0x3C

##Write two bytes to the register.
bus.write_byte_data(ADDRESS, 0x00, 0x80)

##Read two bytes from register 01, the congif register.
##config = bus.read_word_data(ADDRESS, 0x01) & 0xFFFF
##print('Config value: 0x{0:04X}'.format(config))

##Read two bytes from register 00, the ADC value.
##value = bus.read_word_data(ADDRESS,0x00) & 0xFFFF
##print('Raw ADC value: 0x{0:04X}'.format(value))
