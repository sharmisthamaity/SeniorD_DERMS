#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "stdio.h"
#include "SPI.h"
#include "utilities.h"
#include "ADC.h"
#include "PWM.h"


uint32_t duty[24] = {9724,9512,9379,9333,9379,9512,9724,10000,10321,10666,11011,11333,11609,11821,11954,11999,11954,11821,11609,11333,11011,10666,10321};
 

int main(void){
	
  PLL_Init(Bus80MHz);                      // 80 MHz system clock
  heartbeat();
	SPI_Init();
  
	
	//Chopper #1 PWM: PB4 and PB5
	PWM0A2_Init(2000, 1500);	//75% duty cycle, change second number to % of first
	PWM0B2_Init(2000, 1500);
	
	//Chopper #2 PWM: PB6 and PB7
	PWM0A_Init(2000, 1000);	//50% duty cycle, change second number to % of first
	PWM0B_Init(2000, 1000);
	

	//PE4 and PE5
	//PWM0A3_Init(8000, 2000);	//25% duty cycle, change second number to % of first
	//PWM0B3_Init(8000, 2000);
	
	ADC_Init(13333); //ADC channel 0, 6kHz sampling
	ADC0_SAC_R = ADC_SAC_AVG_2X;
	
  EnableInterrupts();
  while(1){

		/*
		index = (index +1)%24;
		PWM0A2_Duty(duty[index]);
		PWM0B2_Duty(duty[index]);
		*/ 
		
		//DelayWait10ms(2000);
   // WaitForInterrupt();
    PF2 ^= 0x04;             // toggle LED

	
  }
}
