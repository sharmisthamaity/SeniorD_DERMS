#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "state.h"
#include "utilities.h"
#include "ADC.h"
#include "PWM.h"

static State s;

void State_Init(void){
	s.current_case = 0;
	s.time = 0;
  s.emergency = 0;
  s.season = 0;
	s.previous_season = 0;
  s.previous_t = 1;
	s.duty = summer_duty;
	
}

void State_process(uint16_t spi_data){
	s.current_case = spi_data&0x0300;
	s.previous_t = s.time;
	s.time = spi_data&0x001F;
	s.emergency = spi_data&0x0020;
	s.previous_season = s.season;
	s.season = spi_data&0xC000;
	
	
	if (s.season != s.previous_season){
	switch(s.season){
		case 0: 
			s.duty = summer_duty;
			break;
		case 1: 
			s.duty = summer_duty;
			break;
		case 2: 
			s.duty = winter_duty;
			break;
		case 3:
			s.duty = spring_duty;
			break;
	}
	s.previous_season = s.season;
}
	
	if (s.emergency){
		PWM0A2_Duty(s.duty[0]);
		PWM0B2_Duty(s.duty[0]);
	}
	else if (s.time != s.previous_t){
		PWM0A2_Duty(s.duty[s.time]);
		PWM0B2_Duty(s.duty[s.time]);
		s.previous_t = s.time;
	}
}

