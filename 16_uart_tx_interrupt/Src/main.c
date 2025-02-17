#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"

#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)
char key;

static void uart_callback(void);


int main(void)
{
	
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER &=~(1U<<11);
	GPIOA->MODER |= (1U<<10);


	while(1)
	{
		printf("Hello from stm32f401re...\n\r");
	}
}

static void uart_callback(void)
{
	key = USART2->DR;
	if(key == '1')
	{
		GPIOA->ODR |= (1U<<5);
		printf("LED on...\n\r");
	}
	else if(key == '2')
	{
		GPIOA->ODR &=~(1U<<5);
		printf("LED off...\n\r");
	}
	else{}
}


void USART2_IRQHandler(void)
{
	/*Check if RXNE is set*/
	if(USART2->SR & SR_RXNE)
	{
		uart_callback();
	}
}



