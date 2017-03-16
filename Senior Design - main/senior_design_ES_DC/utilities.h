#include <stdint.h>

#define PF2       (*((volatile uint32_t *)0x40025010))
	
//PortA	0x40004000  
//PortB	0x40005000
//PortC	0x40006000
//PortD	0x40007000
//PortE	0x40024000
//PortF	0x40025000

/*
7 0x0200
6 0x0100
5 0x0080
4 0x0040
3 0x0020
2 0x0010
1 0x0008
0 0x0004
*/

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void DelayWait10ms(uint32_t n);
void heartbeat(void);
