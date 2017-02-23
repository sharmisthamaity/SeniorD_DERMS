##Overall Algorithm Test

import smbus
import time

BUS = 1
bus = smbus.SMBus(BUS)
loadDevice_ADDRESS = 0x3c
esDevice_ADDRESS = ## need address of other TM4C controller
pvDevice_ADDRESS = ## need address of other TM4C controller

##normal condition
## read load demand
## read power from transmission side - omg what.
## read power from PV
## read load consumption 
## read ES capacity - hmph.
## read line voltage

##CONSTANTS
systemVA = ##distribution grid's apparent power and the limit we want to keep peak at
systemVoltage = ##what's the rms voltage of our grid? 12? 14?

stateOfSystem = 1;

while(stateOfSystem = 1):
    loadDemand = bus.read_byte_data(loadDevice_ADDRESS, 0x01) ##parse number of loadDemand
    transPower = ##power from transmission side (in units of VA? VAR? kW?)
    pvPower = bus.read_byte_data(pvDevice_ADDRESS, 0x01)
    loadConsumption = bus.read_byte_data(loadDevice_ADDRESS, 0x01) ##parse number of loadConsumption
    esCapacity = bus.read_byte_data(esDevice_ADDRESS, 0x01) ##is reading capacity possible?
    lineVoltage = bus.read_byte_data(loadDevice_ADDRESS, 0x01) ##parse number for lineVoltage

    
    ##Peak Shaving
    if(transPower > (0.1*systemVA)):
        ##if the transmission power used > 10% of systemVA(50 VA), then we peak shave
        peakShave(transPower, esCapacity, pvPower, loadDemand)

    ##Reverse Power Flow
    if(pvPower > loadDemand):
        ##if the PV is providing more power than the load needs, then we need to prevent reverse power flow

    ##Voltage Regulation
    if(lineVoltage < (0.9) OR (lineVoltage > (1.05))): ##need to find the OR symbol...keyboard struggles
        ##if the lineVoltage is outside of our range, then we need to regulate voltage

    
    
    

##Peak Shaving Algorithm
def peakShave(transPower, esCapacity, pvPower, loadDemand):
    while(transPower > (0.1*systemVA)): #do i need to pass in systemVA variable? or can this function access it
        ##if pvPower at transmission line < pvPower at PV), then we communicate to PV Inverter to increase ma
        if(transPower < pvPower):
            signalPV = ##some signal value to increase ma
            bus.write_byte_data(pvDevice_ADDRESS, 0x01, signalPV)

        ##Communicate to ES Controller to discharge by some amount
            ##this part was confusing, refer to overall algo pseudo-code later

    return
        

##Reverse Power Flow Algorithm
def revPowFlow(esCapacity, extraPV): ##need to determine inputs
    if((esCapacity < fullCapacity) && pvPower is generating): ##"pvPower is generating" how to determine?
        while(pvPower is gen)
            ##
        
    

##Voltage Regulation Algorithm

