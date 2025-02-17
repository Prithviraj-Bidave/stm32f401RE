#include "stm32f4xx.h"

#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)




int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);

	while(1)
	{
		GPIOA->BSRR |= (1U<<5);
		for(int i =0; i < 300000; i++);
		GPIOA->BSRR |= (1U<<21);
		for(int i =0; i < 300000; i++);
	}
}
