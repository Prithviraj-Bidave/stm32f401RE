#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define SYSTICK_LOAD_VAL			(16000000)
#define CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<2)
#define CTRL_COUNTFLAG				(1U<<16)

#define GPIOAEN			(1U<<0)
#define LED_PIN			(1U<<5)


static void tim2_callback(void);



int main(void)
{
	tim2_1hz_interrupt_init();
	uart2_tx_init();

	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER &=~(1U<<11);
	GPIOA->MODER |= (1U<<10);

	while(1)
	{

	}
}


static void tim2_callback(void)
{
	printf("A second passed!!\n\r");
	GPIOA->ODR ^= LED_PIN;
}


void TIM2_IRQHandler(void)
{
	/*Clear Update Interrupt Flag*/
	TIM2->SR &=~SR_UIF;
	/*Do Something*/
	tim2_callback();
}


