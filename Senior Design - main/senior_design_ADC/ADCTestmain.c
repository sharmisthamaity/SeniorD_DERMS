// ADCTestmain.c
// Runs on LM4F120/TM4C123
// Provide a function that initializes Timer0A to trigger ADC
// SS3 conversions and request an interrupt when the conversion
// is complete.
// Daniel Valvano
// May 3, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// center of X-ohm potentiometer connected to PE3/AIN0
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V through X/10-ohm ohm resistor
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADCT0ATrigger.h"
#include "PLL.h"
#include "UART.h"
#include "math.h"
#include "stdio.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//debug code
//
// This program periodically samples ADC0 channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
extern uint8_t ADCflag;
extern uint32_t ADCvalue[100];

#define ADC_SAC_AVG_M           0x00000007  // Hardware Averaging Control
#define ADC_SAC_AVG_OFF         0x00000000  // No hardware oversampling
#define ADC_SAC_AVG_2X          0x00000001  // 2x hardware oversampling
#define ADC_SAC_AVG_4X          0x00000002  // 4x hardware oversampling
#define ADC_SAC_AVG_8X          0x00000003  // 8x hardware oversampling
#define ADC_SAC_AVG_16X         0x00000004  // 16x hardware oversampling
#define ADC_SAC_AVG_32X         0x00000005  // 32x hardware oversampling
#define ADC_SAC_AVG_64X         0x00000006  // 64x hardware oversampling


volatile float sumarr[1000];
volatile uint16_t index2;
void Pcalculate(void){
	uint32_t ADCcopy[100];
	for (int i = 0; i<100; i++){
		ADCcopy[i] = ADCvalue[i];
	}
	double scaledV[100];
	//double scale1 = 2.66666;  //4V input
	double scale1 = 3.73333;  //5.6V input
	
	for (int i = 0; i<100; i++){
		scaledV[i] = (ADCcopy[i] *3.3)/4096;
		scaledV[i] = scaledV[i] - 1.65;
		//scaledV[i] = scaledV[i] * scale1;
	}
	
	
	float sum = 0;
	for (int i = 0; i<100; i++){
		double ui2 = scaledV[i] * scaledV[i];
		//double ui2 = ADCcopy[i] * ADCcopy[i];
		sum = sum + ui2;
	}
	sum = sum /100;
	sum = sqrt(sum);
	
	sum = sum * scale1;
	//sum = sum/3;
	if (index2<1000){
		sumarr[index2] = sum;
		index2 = index2+1;
	}
	else{
		index2 = 1000;
	}
}

int main(void){
	index2 = 0;
  PLL_Init(Bus80MHz);                      // 80 MHz system clock
  SYSCTL_RCGCGPIO_R |= 0x00000020;         // activate port F
  ADC0_InitTimer0ATriggerSeq3(0, 13333); // ADC channel 0, 6 kHz sampling 

	ADC0_SAC_R = ADC_SAC_AVG_2X;
	UART_Init();
  GPIO_PORTF_DIR_R |= 0x04;                // make PF2 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;             // disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;                // enable digital I/O on PF2
                                           // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;                  // disable analog functionality on PF
  GPIO_PORTF_DATA_R &= ~0x04;              // turn off LED
  EnableInterrupts();
  while(1){
   // WaitForInterrupt();
    GPIO_PORTF_DATA_R ^= 0x04;             // toggle LED
		if (ADCflag){
			ADCflag = 0;
			Pcalculate();
		}
	
  }
}

