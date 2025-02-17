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


static void systick_callback(void);


int main(void)
{
	uart2_tx_init();
	systick_1hz_interrupt();


	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER &=~(1U<<11);
	GPIOA->MODER |= (1U<<10);


	while(1)
	{

	}
}


static void systick_callback(void)
{
	printf("A second passed...!!!\n\r");
	GPIOA->ODR ^= LED_PIN;
}


void SysTick_Handler(void)
{
	/*Do Something*/
	systick_callback();
}


