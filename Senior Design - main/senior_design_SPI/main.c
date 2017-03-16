#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#include "PLL.h"

#include "stdio.h"
#include "SPI.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}


int main(void){
	
  PLL_Init(Bus80MHz);                      // 80 MHz system clock
  SYSCTL_RCGCGPIO_R |= 0x00000020;         // activate port F
  GPIO_PORTF_DIR_R |= 0x04;                // make PF2 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;             // disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;                // enable digital I/O on PF2
                                           // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;                  // disable analog functionality on PF
  GPIO_PORTF_DATA_R |= 0x04;              // turn off LED
	SPI_Init();
	
	uint16_t sharmistha = 0x2300;
	uint16_t buffer[10];
	uint8_t i=0;
	
  EnableInterrupts();
  while(1){
		sharmistha = SPI_exchange(0x1000);
		if(i<10){
			buffer[i] = sharmistha;
			i++;
		}
		//DelayWait10ms(2000);
   // WaitForInterrupt();
    GPIO_PORTF_DATA_R ^= 0x04;             // toggle LED

	
  }
}
