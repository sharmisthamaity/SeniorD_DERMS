#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#include "PLL.h"

#include "stdio.h"


//SSI0Clk PA2 (2) clock
//SSI0Fss PA3 (2) frame signal
//SSI0Rx  PA4 (2) receive
//SSI0Tx  PA5 (2) transmit



void SPI_Init(void){
		SYSCTL_RCGCSSI_R |= 0x01;   //Activate SSI0
	SYSCTL_RCGCGPIO_R |= 0X01;  //Activate portA
	while((SYSCTL_PRGPIO_R&0X01) == 0){}
	GPIO_PORTA_AFSEL_R |= 0x3C; //enable alt fuction of PA2 PA3, PA4, PA5
	GPIO_PORTA_DEN_R |= 0x3C; //config PA2, PA3, PA4, PA5 as ssi
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0000FF) + 0x00222200;	//what does this do?	
	GPIO_PORTA_AMSEL_R = 0; //disable analog functionality 
	SSI0_CR1_R = 0x00000004; //disable SSI, set to slave output enabled mode
	//SSI0_CPSR_R = 0x0a; //need to set this so that ssi clk is at right frequancy
	SSI0_CPSR_R = 0x0a;
		SSI0_CR0_R &= ~(0x0000fff0); //SCR = 0, SPH = 0, SPO = 0, *********might need to change based off of data sheet for dac **try SPH = 1, SPO = 0 
	//SSI0_CR0_R |= 0x00000080; //set SPH to 1;
	SSI0_CR0_R |= 0x0f; //DSS = 16bit data *might need to change*
	SSI0_CR1_R |= 0x00000002; //enable SSI
	
	
	
}

uint16_t SPI_exchange(uint16_t data){
	while ((SSI0_SR_R&SSI_SR_TFE)==0){}
		SSI0_DR_R = data;
	while ((SSI0_SR_R&SSI_SR_RNE) == 0){}
		return SSI0_DR_R;
}

void SPI_send(uint16_t data){
	while ((SSI0_SR_R&SSI_SR_TFE)==0){}
		SSI0_DR_R = data;
}

uint16_t SPI_receive(void){
	while ((SSI0_SR_R&SSI_SR_RNE) == 0){}
		return SSI0_DR_R;
}

