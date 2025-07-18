#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"


int timestamp = 0;

int main(void)
{
	tim2_pa5_output_compare();
	tim3_pa6_input_capture();

	while(1)
	{
		/*Wait until edge is captured*/
		while(!(TIM3->SR & SR_CC1IF));
		/*Read captured value*/
		timestamp = TIM3->CCR1;
	}
}





