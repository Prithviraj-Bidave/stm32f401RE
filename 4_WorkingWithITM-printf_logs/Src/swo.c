#include "swo.h"
#include "stm32f4xx.h"
#include <stdio.h>


int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

void log_error(char *p)
{
	printf("log Error: ");
	printf((char *)p);
}
