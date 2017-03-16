#include <stdint.h>

//initializes SPI
void SPI_Init(void);

//sends and receives SPI data
uint16_t SPI_exchange(uint16_t data);
