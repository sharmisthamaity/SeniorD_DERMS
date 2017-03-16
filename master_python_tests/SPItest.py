import wiringpi
print('done import')

SPIchannel = 1 #SPI Channel (CE1)
SPIspeed = 8000000 #Clock Speed in Hz (8MHz)
print('done channel and speed set')

wiringpi.wiringPiSetupGpio()
print('done setup 2')

wiringpi.wiringPiSPISetup(SPIchannel, SPIspeed)
print('done setup 3')

sendData = str(42) #will send TWO bytes, a byte 4 and a byte 2
print('done sending')
recvData = wiringpi.wiringPiSPIDataRW(SPIchannel, sendData)
print('done receiving')
print(recvData)

#recvData now holds a list [NumOfBytes, recvDataStr]

#alternatively, to send a single byte:
#sendData = chr(42) #will send a single byte containing 42
#recvData = wiringpi.wiringPiSPIDataRW(SPIchannel, sendData)
#recvData is again a list

