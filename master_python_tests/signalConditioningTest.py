##Signal Conditioning Calculation

##One Period vRMS Calculation 
##read in value from i2c.
##store values in an array.
##100 values- calculation.
##scale - 3.73333
##for i = 0 to 100
##scaled[i] = (value*3.3)/4096
##scaled[i] = scaled[i] - 1.65
##end for loop
##sum = 0
##for i = 0 to 100
##ui2 = scaled[i]*scaled[i]
## sum = sum + ui2
##end for loop
##sum = sum/100
## sum = sqrt(sum)
## sum = sum*scale

##store for 1000 periods
##print out

import smbus

BUS = 1
bus = smbus.SMBus(BUS)
ADDRESS = #need device address

onePeriodVRMS = []
allPeriodsVRMS = []

for periodCount in range(1,1000):
    for i in range(1,100):
        value = bus.read_word_data(ADDRESS, 0x01)
        onePeriodVRMS.append(value)

    ##start calculation!
    scale = 3.73333
    ui2List = []
    for i in range(1,100):
        readValue = onePeriodVRMS[i]
        newValue = ((readValue*3.3)/4096)-1.65
        ui2List.append(newValue*newValue)

    periodSum = sum(ui2List)    
    scaledSum = sqrt(periodSum/100)*scale

    #store sum in array    
    allPeriodsVRMS.append(scaledSum)

print(allPeriodsVRMS)

        
    
    
    
