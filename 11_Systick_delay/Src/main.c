#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define SYSTICK_LOAD_VAL			(16000000)
#define CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<2)
#define CTRL_COUNTFLAG				(1U<<16)

#define GPIOAEN			(1U<<0)
#define LED_PIN			(1U<<5)
int main(void)
{
	uart2_tx_init();
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER &=~(1U<<11);
	GPIOA->MODER |= (1U<<10);
	SysTick->LOAD = (SYSTICK_LOAD_VAL);
	SysTick->VAL = 0;
	SysTick->CTRL = (CTRL_ENABLE | CTRL_CLKSRC);

	while(1)
	{
		for(int i = 0; i < 5;)
		{
			if(SysTick->CTRL & CTRL_COUNTFLAG)
			{
				GPIOA->ODR ^= LED_PIN;
				SysTick->CTRL = 0;
				SysTick->LOAD = (SYSTICK_LOAD_VAL);
				SysTick->VAL = 0;
				SysTick->CTRL = (CTRL_ENABLE | CTRL_CLKSRC);
				i++;
			}
		}
		printf("5 seconds passed!!\n\r");
	}
}





