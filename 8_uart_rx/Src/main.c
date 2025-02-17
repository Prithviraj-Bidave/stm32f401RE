#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"

#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)
char key;

int main(void)
{
	uart2_rxtx_init();
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER &=~(1U<<11);
	GPIOA->MODER |= (1U<<10);


	while(1)
	{
		key = uart2_read();
		if(key == 's')
		{
			GPIOA->ODR |= LED_PIN;
			printf("Input: s");
		}
		else if(key == 'h')
		{
			GPIOA->ODR &=~LED_PIN;
			printf("Input: h");
		}
		else{}
	}
}





