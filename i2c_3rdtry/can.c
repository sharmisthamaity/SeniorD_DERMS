
// slave_receive_int.c - Example demonstrating a simple I2C master message
//                       transmission using a slave interrupt when data is
//                       received.


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


//*****************************************************************************
//
//! \addtogroup i2c_examples_list
//! <h1>Slave Receive Interrupt (slave_receive_int)</h1>
//!
//! This example shows how to configure a receive interrupt on the slave
//! module.  This includes setting up the I2C0 module for loopback mode as well
//! as configuring the master and slave modules.  Loopback mode internally
//! connects the master and slave data and clock lines together.  The address
//! of the slave module is set to a value so it can receive data from the
//! master.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - I2C0 peripheral
//! - GPIO Port B peripheral (for I2C0 pins)
//! - I2C0SCL - PB2
//! - I2C0SDA - PB3
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of I2C.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - INT_I2C0 - I2C0SlaveIntHandler
//
//*****************************************************************************

//*****************************************************************************
//
// Set the address for slave module. This is a 7-bit address sent in the
// following format:
//                      [A6:A5:A4:A3:A2:A1:A0:RS]
//
// A zero in the R/S position of the first byte means that the master
// transmits (sends) data to the selected slave, and a one in this position
// means that the master receives data from the slave.
//
//*****************************************************************************
#define SLAVE_ADDRESS           0x3C

//*****************************************************************************
//
// Global variable to hold the I2C data that has been received.
//
//*****************************************************************************
static uint32_t data_storage=0x99;
static uint32_t throwaway=0x88;

//*****************************************************************************
//
// This is a flag that gets set in the interrupt handler to indicate that an
// interrupt occurred.
//
//*****************************************************************************
static bool g_bIntFlag = false;


//*****************************************************************************
//
// The interrupt handler for the for I2C0 data slave interrupt.
//
//*****************************************************************************
/*void
I2C0SlaveIntHandler(void)
{
    //
    // Clear the I2C0 interrupt flag.
    //
	I2CSlaveIntClear(I2C0_BASE);

    //
    // Read the data from the slave.
    //
	data_storage = I2CSlaveDataGet(I2C0_BASE);

    //
    // Set a flag to indicate that the interrupt occurred.
    //
	g_bIntFlag = true;
}
*/

int
main(void){
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
	defined(TARGET_IS_TM4C129_RA1) ||                                         \
	defined(TARGET_IS_TM4C129_RA2)
	uint32_t ui32SysClock;
#endif
	uint32_t ui32DataTx;

    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
	defined(TARGET_IS_TM4C129_RA1) ||                                         \
	defined(TARGET_IS_TM4C129_RA2)
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
		SYSCTL_OSC_MAIN |
		SYSCTL_USE_PLL), 25000000);
#else
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
		SYSCTL_XTAL_16MHZ);
#endif

    //
    // The I2C0 peripheral must be enabled before use.
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //
    // For this example I2C0 is used with PortB[3:2].  The actual port and
    // pins used may be different on your part, consult the data sheet for
    // more information.  GPIO port B needs to be enabled so these pins can
    // be used.
    // TODO: change this to whichever GPIO port you are using.
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //
    // Select the I2C function for these pins.  This function will also
    // configure the GPIO pins pins for I2C operation, setting them to
    // open-drain operation with weak pull-ups.  Consult the data sheet
    // to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable the GPIO PortF
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		// Set the GPIO PortF Pin2 as output
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    //
    // Enable the I2C0 interrupt on the processor (NVIC).
    //
	//IntEnable(INT_I2C0);

    //
    // Configure and turn on the I2C0 slave interrupt.  The I2CSlaveIntEnableEx()
    // gives you the ability to only enable specific interrupts.  For this case
    // we are only interrupting when the slave device receives data.
    //
	//I2CSlaveIntEnableEx(I2C0_BASE, I2C_SLAVE_INT_DATA);

    //
    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.  For this example we will use a data rate of 100kbps.
    //
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
	defined(TARGET_IS_TM4C129_RA1) ||                                         \
	defined(TARGET_IS_TM4C129_RA2)
  //  I2CMasterInitExpClk(I2C0_BASE, ui32SysClock, false);
#else
   // I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
#endif

    //
    // Enable the I2C0 slave module.
    //
	I2CSlaveEnable(I2C0_BASE);

    //
    // Set the slave address to SLAVE_ADDRESS.  In loopback mode, it's an
    // arbitrary 7-bit number (set in a macro above) that is sent to the
    // I2CMasterSlaveAddrSet function.
    //
	I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);

    //
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.  Set the address to SLAVE_ADDRESS
    // (as set in the slave module).  The receive parameter is set to false
    // which indicates the I2C Master is initiating a writes to the slave.  If
    // true, that would indicate that the I2C Master is initiating reads from
    // the slave.
    //
   // I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);

    //
    // Set up the serial console to use for displaying messages.  This is just
    // for this example program and is not needed for proper I2C operation.
    //
  //  InitConsole();

    //
    // Enable interrupts to the processor.
    //
			//IntMasterEnable();

  
    //I2CMasterDataPut(I2C0_BASE, ui32DataTx);

    //
    // Initiate send of single piece of data from the master.  Since the
    // loopback mode is enabled, the Master and Slave units are connected
    // allowing us to receive the same data that we sent out.
    //
    //I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);


//--------------------------RECIEVING DATA FROM RP3------------
        // Wait until the slave has received and acknowledged the data.
        //
        while(!(I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_RREQ))
        {
        }
				//then read data from RP3
        data_storage = I2CSlaveDataGet(I2C0_BASE);
				

//--------------------------SENDING DATA TO RP3-------------------
//wait til the master is ready to recieve
				while(!(I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_TREQ))
				{
				}
//send data to master
				I2CSlaveDataPut(I2C0_BASE, 'j');
				//I2CSlaveDataPut(I2C0_BASE, data_storage);
uint8_t d = data_storage & 0xFF;
				
	while(1)
	{
		/*	if ((I2CSlaveStatus(I2C0_BASE) && I2C_SLAVE_ACT_RREQ)){
				data_storage = I2CSlaveDataGet(I2C0_BASE);
			}
		else if (I2CSlaveStatus(I2C0_BASE) && I2C_SLAVE_ACT_TREQ)
		{
				I2CSlaveDataPut(I2C0_BASE, 'd');
		}*/
	}
	{
		
		
		
	}
}
