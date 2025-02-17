#include "stm32f4xx.h"

#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)

#define GPIOCEN		(1U<<2)
#define BTN_PIN		(1U<<13)



int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);

	RCC->AHB1ENR |= GPIOCEN;
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

	while(1)
	{
		if(GPIOC->IDR & (0x2000))
		{
			GPIOA->BSRR |= (1U<<5);
		}
		else
		{
			GPIOA->BSRR |= (1U<<21);
		}
	}
}
