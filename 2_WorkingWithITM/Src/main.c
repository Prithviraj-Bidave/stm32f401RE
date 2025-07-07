#include "stm32f4xx.h"
#include <string.h>


#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)




char _a[] = "Hello from stm32f401re";
char *pt_a = _a;

int main(void)
{
	for(int i = 0; i < strlen(_a); i++)
	{
		ITM_SendChar(*pt_a++);
	}

	while(1)
	{

	}
}
