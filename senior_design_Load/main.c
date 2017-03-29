#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "stdio.h"
#include "SPI.h"
#include "utilities.h"
#include "ADC.h"
#include "PWM.h"
#include "state.h"

extern volatile uint32_t data[8];
extern uint8_t ADC_flag;

uint32_t duty[24] = {9724,9512,9379,9333,9379,9512,9724,10000,10321,10666,11011,11333,11609,11821,11954,11999,11954,11821,11609,11333,11011,10666,10321};
 

int main(void){
	
  PLL_Init(Bus80MHz);                      // 80 MHz system clock
  heartbeat();
	PF3 = 0x08;															//Initialization is occuring
	SPI_Init();
  
	
	//Chopper #1 PWM: PB4 and PB5
	//PWM0A2_Init(2000, 1500);	//75% duty cycle, change second number to % of first
	//PWM0B2_Init(2000, 1500);
	
	//Chopper #2 PWM: PB6 and PB7
	//PWM0A_Init(2000, 1000);	//50% duty cycle, change second number to % of first
	//PWM0B_Init(2000, 1000);
	

	//PE4 and PE5
	//PWM0A3_Init(8000, 2000);	//25% duty cycle, change second number to % of first
	//PWM0B3_Init(8000, 2000);
	
	ADC_Init(13333); //ADC channel 0, 6kHz sampling
	
	uint16_t m_data;
	
	PF3 = 0; //Initialization Complete
  EnableInterrupts();
  while(1){
	//DelayWait10ms(10);
		//ADC_In();
		if (ADC_flag){
			SPI_send(data[0]);
			SPI_send(data[1]);
			SPI_send(data[2]);
			SPI_send(data[3]);
			//SPI_send(data[4]);
			ADC_flag = 0;
		}
		
		if ((SSI0_SR_R&SSI_SR_RNE) == 1){
		m_data = SSI0_DR_R&0xFF;
			State_process(m_data);
		//process_data(m_data)
	}	
		
    PF2 ^= 0x04;             // toggle LED
	
  }
}
