#include "stm32f4xx.h"



#define TIM2EN		(1U<<0)
#define CR1_CEN		(1U<<0)
#define OC_TOGGLE	((1U<<4) | (1U<<5))
#define CCER_CC1E	(1U<<0)

void tim2_1hz_init(void)
{
	/*Enable clock access to tim2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set the prescaler value*/
	TIM2->PSC = 1600 - 1;
	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;
	/*Clear counter*/
	TIM2->CNT = 0;
	/*Enable timer*/
	TIM2->CR1 = CR1_CEN;
}



void tim2_pa5_output_compare(void)
{
	/*Enable Clock Access*/
	RCC->AHB1ENR |= (1U<<0);

	/*Set to alternate function mode*/
	GPIOA->MODER |= (1U<<11);
	GPIOA->MODER &=~(1U<<10);

	/*Set alternate function type to AF01*/
	GPIOA->AFR[0] &=~(1U<<23);
	GPIOA->AFR[0] &=~(1U<<22);
	GPIOA->AFR[0] &=~(1U<<21);
	GPIOA->AFR[0] |= (1U<<20);

	/*Enable clock access to tim2*/
	RCC->APB1ENR |= TIM2EN;

	/*Set the prescaler value*/
	TIM2->PSC = 2 - 1;
	/*Set auto-reload value*/
	TIM2->ARR = 2 - 1;

	/*Set output compare toggle mode*/
	TIM2->CCMR1 = OC_TOGGLE;
	/*Enable tim2 ch1 in compare mode*/
	TIM2->CCER |= CCER_CC1E;


	/*Clear counter*/
	TIM2->CNT = 0;
	/*Enable timer*/
	TIM2->CR1 = CR1_CEN;
}
