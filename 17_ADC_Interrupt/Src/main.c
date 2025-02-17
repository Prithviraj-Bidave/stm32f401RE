#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"

static void adc_calback(void);

uint32_t sensor_value;


int main(void)
{
	uart2_tx_init();
	pa1_adc_interrupt_init();
	start_conversion();
	while(1)
	{

	}
}

static void adc_calback(void)
{
	sensor_value = ADC1->DR;
	printf("Sensor Value : %d\n\r", (int)sensor_value);
}

void ADC_IRQHandler(void)
{
	/*Check for EOC in SR*/
	if(ADC1->SR & SR_EOC)
	{
		/*Clear EOC*/
		ADC1->SR &=~SR_EOC;
		/*Do Something*/
		adc_calback();
	}
}
