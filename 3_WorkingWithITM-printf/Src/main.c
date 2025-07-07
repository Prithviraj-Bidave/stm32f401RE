#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)




int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

int main(void)
{
	while(1)
	{
		printf("printf is now available\n");
	}
}
