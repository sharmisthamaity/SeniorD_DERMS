#include <stdint.h>
#include "PLL.h"
#include "PWM.h"
#include "../inc/tm4c123gh6pm.h"

#define PE0       (*((volatile uint32_t *)0x40024004))
#define PE1       (*((volatile uint32_t *)0x40024008))

//array

uint32_t duty[24] = {9724,9512,9379,9333,9379,9512,9724,10000,10321,10666,11011,11333,11609,11821,11954,11999,11954,11821,11609,11333,11011,10666,10321};
 



int main2(void){
	PLL_Init(Bus80MHz);               // bus clock at 80 MHz
  //PWM0A_Init(13333, 6666);         // initialize PWM0, 6000 Hz, 50% duty
	//PWM0B_Init(13333, 6666);
	
	//PWM0A_Init(2000, 1000);
	//PWM0B_Init(2000, 1000);
	
	//PB6 and PB7
	PWM0A_Init(2000, 1000);	//50% duty cycle, change second number to % of first
	PWM0B_Init(2000, 1000);
	
	//PB4 and PB5
	PWM0A2_Init(2000, 1500);	//75% duty cycle, change second number to % of first
	PWM0B2_Init(2000, 1500);
	
	//PE4 and PE5
	//PWM0A3_Init(8000, 2000);	//25% duty cycle, change second number to % of first
	//PWM0B3_Init(8000, 2000);
	
	uint8_t index = 0;
	
	SYSCTL_RCGCGPIO_R |= 0x00000020;         // activate port F

	GPIO_PORTF_DIR_R |= 0x04;                // make PF2 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;             // disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;                // enable digital I/O on PF2
                                           // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;                  // disable analog functionality on PF
	
	
  GPIO_PORTF_DATA_R &= ~0x18;              // turn off LED
	
	while(1){
		/*DelayWait10ms(100);
		index = (index +1)%24;
		PWM0A2_Duty(duty[index]);
		PWM0B2_Duty(duty[index]);*/
		GPIO_PORTF_DATA_R ^= 0x04;             // toggle LED

	}
}
