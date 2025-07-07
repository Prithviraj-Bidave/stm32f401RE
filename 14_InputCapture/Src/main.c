#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"


int timestamp1 = 0, timestamp2 = 0;



int main(void)
{
//	tim2_pa5_output_compare();
//	tim3_pa6_input_capture();
	uart2_rxtx_init();
	tim2_pa5_pa1_output_compare();
	tim3_pa6_pa7_input_capture();

	while(1)
	{
		/*Wait until edge is captured*/
		while(!(TIM3->SR & SR_CC1IF));
		/*Read captured value*/
		timestamp1 = TIM3->CCR1;

		/*Wait until edge is captured*/
		while(!(TIM3->SR & SR_CC2IF));
		/*Read captured value*/
		timestamp2 = TIM3->CCR2;

		if(timestamp1 > timestamp2)
		{
			printf("Timestamp 1\n\r");
		}
		else
		{
			printf("Timestamp 2\n\r");
		}
	}
}





