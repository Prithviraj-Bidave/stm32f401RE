#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

static void exti_callback(void);


int main(void)
{
	pc13_exti_init();
	uart2_tx_init();
	tim2_1hz_init();

	RCC->AHB1ENR |= (1U<<0);

	GPIOA->MODER &=~(1U<<11);
	GPIOA->MODER |= (1U<<10);


	while(1)
	{
//		while(!(TIM2->SR & SR_UIF));
//		TIM2->SR &=~SR_UIF;
//		GPIOA->ODR ^= (1U<<5);

	}
}


static void exti_callback(void)
{
	printf("BTN pressed....\n\r");
	GPIOA->ODR |= (1U<<5);
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & LINE13)
	{
		/*Clear PR Flag*/
		EXTI->PR |= LINE13;
		/*Do Something*/
		exti_callback();
	}
}




