#include <stdint.h>

//initializes SPI
void SPI_Init(void);

//sends and receives SPI data
uint16_t SPI_exchange(uint16_t data);

//sends SPI data
void SPI_send(uint16_t data);

//receives SPI data
uint16_t SPI_receive(void);

//sends SPI data
void SPI_send2(uint16_t data);

//receives SPI data
uint16_t SPI_receive2(void);

