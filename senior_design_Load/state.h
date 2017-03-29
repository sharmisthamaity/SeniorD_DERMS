#include <stdint.h>

static uint32_t summer_duty[24] = {968,901,866,919,1000,1061,1173,1275,1346,1414,1458,1521,1620,1696,1732,1750,1768,1732,1677,1581,1458,1323,1146,1031};
static uint32_t winter_duty[24] = {1350,1240,1200,1240,1350,1500,1650,1760,1800,1760,1650,1500,1350,1240,1200,1240,1350,1500,1650,1760,1800,1760,1650,1500};
static uint32_t spring_duty[24] = {552,470,400,346,312,300,312,346,400,470,552,640,728,810,880,934,968,980,968,934,880,810,728,640};	


typedef struct State{
	uint8_t current_case;
	uint8_t time;
	uint8_t emergency;
	uint8_t season;
	uint8_t previous_season;
	uint16_t previous_t;
	uint32_t *duty;
} State;

void State_process(uint16_t spi_data);
void State_Init(void);
