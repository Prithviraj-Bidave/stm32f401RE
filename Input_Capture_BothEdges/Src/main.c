#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"


int timestamp = 0, timestamp2 = 0, time_period = 0;

int main(void)
{
	tim2_pa5_output_compare();
//	tim3_pa6_input_capture();
//	uart2_tx_init();
//	printf("Initializing System\n\r");
//	while(1)
//	{
//		/*Wait until edge is captured*/
//		while(!(TIM3->SR & SR_CC1IF));
//		/*Read captured value*/
//		timestamp = TIM3->CCR1;
////		printf("TimeStamp1: %d\n\r", timestamp);
//		while(!(TIM3->SR & SR_CC1IF));
//		/*Read captured value*/
//		timestamp2 = TIM3->CCR1;
////		printf("TimeStamp2: %d\n\r", timestamp2);
//		time_period = timestamp2 - timestamp;
////		printf("TimePeriod: %d\n\r", time_period);
//	}
}





