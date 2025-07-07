#include "stm32f4xx.h"

#define GPIOAEN		(1U<<0)
#define LED_PIN		(1U<<5)



int total = 0;

int main(void)
{
	for(int i = 0; i < 100000; i++)
	{
		total +=i;
	}
	while(1){}
}
