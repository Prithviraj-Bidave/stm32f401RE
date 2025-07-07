#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adxl345.h"


#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)

int16_t x,y,z;
double xg,yg,zg;

extern uint8_t data_rec[6];
const float FOUR_G_SCALE_FACT = 0.0078;


int main(void)
{
	uart2_rxtx_init();
	adxl_init();
	printf("System Initializing\n\r");

	while (1)
	{
		adxl_read_values(DATA_START_ADDR);
		x = ((data_rec[1]<<8)|data_rec[0]);
		y = ((data_rec[3]<<8)|data_rec[2]);
		z = ((data_rec[5]<<8)|data_rec[4]);

		xg = x * 0.0078;
		yg = y * 0.0078;
		zg = z * 0.0078;
	}

}



