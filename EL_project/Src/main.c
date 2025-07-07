#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include <stdlib.h>


#define GPIOAEN		(1U<<0)
#define GPIOBEN		(1U<<1)
#define LED_PIN		(1U<<5)
char key[8];
uint8_t num;
uint8_t n = 0;

//static void uart_callback(void);
uint8_t in, flag;

uint8_t lights[8] = {0,1,1,1,1,1,1,1};



int main(void)
{
	uart2_rxtx_init();
	RCC->AHB1ENR |= GPIOAEN;
	RCC->AHB1ENR |= GPIOBEN;


	GPIOB->MODER &=~(1U<<25);
	GPIOB->MODER |= (1U<<24);

	GPIOB->MODER &=~(1U<<21);
	GPIOB->MODER |= (1U<<20);

	GPIOB->MODER &=~(1U<<19);
	GPIOB->MODER |= (1U<<18);

	GPIOB->MODER &=~(1U<<17);
	GPIOB->MODER |= (1U<<16);

	GPIOB->MODER &=~(1U<<15);
	GPIOB->MODER |= (1U<<14);

	GPIOB->MODER &=~(1U<<13);
	GPIOB->MODER |= (1U<<12);

	GPIOB->MODER &=~(1U<<11);
	GPIOB->MODER |= (1U<<10);

	GPIOB->MODER &=~(1U<<9);
	GPIOB->MODER |= (1U<<8);

	GPIOB->MODER &=~(1U<<7);
	GPIOB->MODER |= (1U<<6);

	GPIOB->MODER &=~(1U<<5);
	GPIOB->MODER |= (1U<<4);

	GPIOB->MODER &=~(1U<<3);
	GPIOB->MODER |= (1U<<2);

	GPIOB->MODER &=~(1U<<1);
	GPIOB->MODER |= (1U<<0);


	while(1)
	{
		key[in] = uart2_read();
		if(key[in]=='\n')
		{
			num = atoi(key);
			in = 0;

		}
		else
		{
			in++;
		}
		lights[n] = num;

		// Increment only when num is 1 and the flag is not set
		if(num == 1 && flag == 0)
		{
			printf("Incrementing n: %d\n", n);  // Debug print
			n++;
			if(n == 8)
			{
				n = 0;
			}
			flag = 1;  // Set flag to prevent further increments until num changes
		}
		else if(num != 1)
		{
			flag = 0;  // Reset flag if num is not 1
		}

		if(lights[0] == 1)
		{
			GPIOB->ODR &=~(1U<<0);
			if(lights[1] == 1)
			{
				GPIOB->ODR &=~(1U<<1);
				GPIOB->ODR |= (1U<<2);
			}
			else
			{
				GPIOB->ODR |= (1U<<1);
				GPIOB->ODR &=~(1U<<2);
			}
		}
		else
		{
			GPIOB->ODR |= (1U<<0);
			GPIOB->ODR &=~(1U<<1);
			GPIOB->ODR &=~(1U<<2);
		}


		if(lights[2] == 1)
		{
			GPIOB->ODR &=~(1U<<3);
			if(lights[3] == 1)
			{
				GPIOB->ODR &=~(1U<<4);
				GPIOB->ODR |= (1U<<5);
			}
			else
			{
				GPIOB->ODR |= (1U<<4);
				GPIOB->ODR &=~(1U<<5);
			}
		}
		else
		{
			GPIOB->ODR |= (1U<<3);
			GPIOB->ODR &=~(1U<<4);
			GPIOB->ODR &=~(1U<<5);
		}


		if(lights[4] == 1)
		{
			GPIOB->ODR &=~(1U<<6);
			if(lights[5] == 1)
			{
				GPIOB->ODR &=~(1U<<7);
				GPIOB->ODR |= (1U<<8);
			}
			else
			{
				GPIOB->ODR |= (1U<<7);
				GPIOB->ODR &=~(1U<<8);
			}
		}
		else
		{
			GPIOB->ODR |= (1U<<6);
			GPIOB->ODR &=~(1U<<7);
			GPIOB->ODR &=~(1U<<8);
		}
		if(lights[6] == 1)
		{
			GPIOB->ODR &=~(1U<<9);
			if(lights[7] == 1)
			{
				GPIOB->ODR &=~(1U<<10);
				GPIOB->ODR |= (1U<<12);
			}
			else
			{
				GPIOB->ODR |= (1U<<10);
				GPIOB->ODR &=~(1U<<12);
			}
		}
		else
		{
			GPIOB->ODR |= (1U<<9);
			GPIOB->ODR &=~(1U<<10);
			GPIOB->ODR &=~(1U<<12);
		}
	}
}


//static void uart_callback(void)
//{
//	key[in] = uart2_read();
//	if(key[in]=='\n')
//	{
//		num = atoi(key);
//		in = 0;
//
//	}
//	else
//	{
//		in++;
//	}
//}
//
//
//void USART2_IRQHandler(void)
//{
//	/*Check if RXNE is set*/
//	if(USART2->SR & SR_RXNE)
//	{
//		uart_callback();
//	}
//}



