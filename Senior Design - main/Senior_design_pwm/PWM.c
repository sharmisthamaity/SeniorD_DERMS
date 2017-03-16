// PWM.c
// Runs on TM4C123
// Use PWM0A/PB6 and PWM0B/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Program 6.8, section 6.3.2

   "Embedded Systems: Real-Time Operating Systems for ARM Cortex M Microcontrollers",
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2015
   Program 8.4, Section 8.3

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
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#define PWM_0_GENA_ACTCMPAD_ONE 0x000000C0  // Set the output signal to 1
#define PWM_0_GENA_ACTLOAD_ZERO 0x00000008  // Set the output signal to 0
#define PWM_0_GENB_ACTCMPBD_ONE 0x00000C00  // Set the output signal to 1
#define PWM_0_GENB_ACTLOAD_ZERO 0x00000008  // Set the output signal to 0

#define SYSCTL_RCC_USEPWMDIV    0x00100000  // Enable PWM Clock Divisor
#define SYSCTL_RCC_PWMDIV_M     0x000E0000  // PWM Unit Clock Divisor
#define SYSCTL_RCC_PWMDIV_2     0x00000000  // /2


// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PB6 and PB7 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PB6/M0PWM0
void PWM0A_Init(uint16_t period, uint16_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
  GPIO_PORTB_AFSEL_R |= 0x40;           // enable alt funct on PB6
  GPIO_PORTB_PCTL_R &= ~0x0F000000;     // configure PB6 as PWM0
  GPIO_PORTB_PCTL_R |= 0x04000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;          // disable analog functionality on PB6
		
		GPIO_PORTB_DR8R_R |=0xC0;	//enable 8mA output
		
  GPIO_PORTB_DEN_R |= 0x40;             // enable digital I/O on PB6
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      (SYSCTL_RCC_R & (~0x000E0000));   //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPA_R = duty - 1;             // 6) count value when output rises
		
	//PWM0_0_DBRISE_R = (PWM0_0_DBRISE_R & 0xFFFFC000) | 0x3FF;
	//PWM0_0_DBFALL_R = (PWM0_0_DBFALL_R & 0xFFFFC000) | 0x3FF;	
	PWM0_0_DBRISE_R = (PWM0_0_DBRISE_R & 0xFFFFC000) | 0x0F;
	PWM0_0_DBFALL_R = (PWM0_0_DBFALL_R & 0xFFFFC000) | 0x0F;	
	PWM0_0_DBCTL_R |= 0x01;			//set first bit to 1

		
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000001;          // enable PB6/M0PWM0
}
// change duty cycle of PB6
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0A_Duty(uint16_t duty){
  PWM0_0_CMPA_R = duty - 1;             // 6) count value when output rises
}
// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PB6 and PB7 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PB7/M0PWM1
void PWM0B_Init(uint16_t period, uint16_t duty){
  volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  delay = SYSCTL_RCGCGPIO_R;            // allow time to finish activating
  GPIO_PORTB_AFSEL_R |= 0x80;           // enable alt funct on PB7
  GPIO_PORTB_PCTL_R &= ~0xF0000000;     // configure PB7 as M0PWM1
  GPIO_PORTB_PCTL_R |= 0x40000000;
  GPIO_PORTB_AMSEL_R &= ~0x80;          // disable analog functionality on PB7
  GPIO_PORTB_DEN_R |= 0x80;             // enable digital I/O on PB7
  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; // 3) use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //    clear PWM divider field
  SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;  //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENB_R = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
  // PB7 goes low on LOAD
  // PB7 goes high on CMPB down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPB_R = duty - 1;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000002;          // enable PB7/M0PWM1
}
// change duty cycle of PB7
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0B_Duty(uint16_t duty){
  PWM0_0_CMPB_R = duty - 1;             // 6) count value when output rises
}




// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PB4 and PB5 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PB4/M0PWM2
void PWM0A2_Init(uint16_t period, uint16_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
  GPIO_PORTB_AFSEL_R |= 0x10;           // enable alt funct on PB4
  GPIO_PORTB_PCTL_R &= ~0x000F0000;     // configure PB4 as PWM0
  GPIO_PORTB_PCTL_R |= 0x00040000;
  GPIO_PORTB_AMSEL_R &= ~0x10;          // disable analog functionality on PB4
		
		GPIO_PORTB_DR8R_R |=0x30;	//enable 8mA output
		
  GPIO_PORTB_DEN_R |= 0x10;             // enable digital I/O on PB4
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      (SYSCTL_RCC_R & (~0x000E0000));   //    configure for /2 divider
  PWM0_1_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_1_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_1_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_1_CMPA_R = duty - 1;             // 6) count value when output rises
		
	//PWM0_0_DBRISE_R = (PWM0_0_DBRISE_R & 0xFFFFC000) | 0x3FF;
	//PWM0_0_DBFALL_R = (PWM0_0_DBFALL_R & 0xFFFFC000) | 0x3FF;	
	PWM0_1_DBRISE_R = (PWM0_1_DBRISE_R & 0xFFFFC000) | 0x0F;
	PWM0_1_DBFALL_R = (PWM0_1_DBFALL_R & 0xFFFFC000) | 0x0F;	
	PWM0_1_DBCTL_R |= 0x01;			//set first bit to 1

		
  PWM0_1_CTL_R |= 0x00000001;           // 7) start PWM0   
	//MAY WANT TO CHECK THAT
  PWM0_ENABLE_R |= 0x00000004;          // enable PB4/M0PWM2
}
// change duty cycle of PB4
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0A2_Duty(uint16_t duty){
  PWM0_1_CMPA_R = duty - 1;             // 6) count value when output rises
}
// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PB4 and PB5 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PB5/M0PWM3
void PWM0B2_Init(uint16_t period, uint16_t duty){
  volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  delay = SYSCTL_RCGCGPIO_R;            // allow time to finish activating
  GPIO_PORTB_AFSEL_R |= 0x20;           // enable alt funct on PB5
  GPIO_PORTB_PCTL_R &= ~0x00F00000;     // configure PB5 as M0PWM3
  GPIO_PORTB_PCTL_R |= 0x00400000;
  GPIO_PORTB_AMSEL_R &= ~0x20;          // disable analog functionality on PB5
  GPIO_PORTB_DEN_R |= 0x20;             // enable digital I/O on PB5
  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; // 3) use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //    clear PWM divider field
  SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;  //    configure for /2 divider
  PWM0_1_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_1_GENB_R = (PWM_1_GENB_ACTCMPBD_ONE|PWM_1_GENB_ACTLOAD_ZERO);
  // PB7 goes low on LOAD
  // PB7 goes high on CMPB down
  PWM0_1_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_1_CMPB_R = duty - 1;             // 6) count value when output rises
  PWM0_1_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000008;          // enable PB5/M0PWM3
}
// change duty cycle of PB5
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0B2_Duty(uint16_t duty){
  PWM0_1_CMPB_R = duty - 1;             // 6) count value when output rises
}






// Output on PE4/M0PWM4
void PWM0A3_Init(uint16_t period, uint16_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x10;            // 2) activate port E
  while((SYSCTL_PRGPIO_R&0x10) == 0){};
  GPIO_PORTE_AFSEL_R |= 0x10;           // enable alt funct on PE4
  GPIO_PORTE_PCTL_R &= ~0x000F0000;     // configure PE4 as PWM0
  GPIO_PORTE_PCTL_R |= 0x00040000;
  GPIO_PORTE_AMSEL_R &= ~0x10;          // disable analog functionality on PE4
		
		GPIO_PORTE_DR8R_R |=0x30;	//enable 8mA output
		
  GPIO_PORTE_DEN_R |= 0x10;             // enable digital I/O on PE4
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      (SYSCTL_RCC_R & (~0x000E0000));   //    configure for /2 divider
  PWM0_2_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_2_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PE4 goes low on LOAD
  // PE4 goes high on CMPA down
  PWM0_2_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_2_CMPA_R = duty - 1;             // 6) count value when output rises
		
	//PWM0_0_DBRISE_R = (PWM0_0_DBRISE_R & 0xFFFFC000) | 0x3FF;
	//PWM0_0_DBFALL_R = (PWM0_0_DBFALL_R & 0xFFFFC000) | 0x3FF;	
	PWM0_2_DBRISE_R = (PWM0_1_DBRISE_R & 0xFFFFC000) | 0x0F;
	PWM0_2_DBFALL_R = (PWM0_1_DBFALL_R & 0xFFFFC000) | 0x0F;	
	PWM0_2_DBCTL_R |= 0x01;			//set first bit to 1

		
  PWM0_2_CTL_R |= 0x00000001;           // 7) start PWM0   
	//MAY WANT TO CHECK THAT
  PWM0_ENABLE_R |= 0x00000010;          // enable PB4/M0PWM4
}
// change duty cycle of PE4
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0A3_Duty(uint16_t duty){
  PWM0_2_CMPA_R = duty - 1;             // 6) count value when output rises
}

// Output on PE5/M0PWM5
void PWM0B3_Init(uint16_t period, uint16_t duty){
  volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x10;            // 2) activate port E
  delay = SYSCTL_RCGCGPIO_R;            // allow time to finish activating
  GPIO_PORTE_AFSEL_R |= 0x20;           // enable alt funct on PB5
  GPIO_PORTE_PCTL_R &= ~0x00F00000;     // configure PB7 as M0PWM1
  GPIO_PORTE_PCTL_R |= 0x00400000;
  GPIO_PORTE_AMSEL_R &= ~0x20;          // disable analog functionality on PB5
  GPIO_PORTE_DEN_R |= 0x20;             // enable digital I/O on PB5
  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; // 3) use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //    clear PWM divider field
  SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;  //    configure for /2 divider
  PWM0_2_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_2_GENB_R = (PWM_2_GENB_ACTCMPBD_ONE|PWM_2_GENB_ACTLOAD_ZERO);
  // PE5 goes low on LOAD
  // PE5 goes high on CMPB down
  PWM0_2_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_2_CMPB_R = duty - 1;             // 6) count value when output rises
  PWM0_2_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000020;          // enable PB5/M0PWM3
}
// change duty cycle of PE5
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0B3_Duty(uint16_t duty){
  PWM0_2_CMPB_R = duty - 1;             // 6) count value when output rises
}




