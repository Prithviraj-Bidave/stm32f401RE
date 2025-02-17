#include "stm32f4xx.h"



#define TIM2EN		(1U<<0)
#define TIM3EN		(1U<<1)

#define CR1_CEN		(1U<<0)
#define OC_TOGGLE	((1U<<4) | (1U<<5))
#define CCER_CC1E	(1U<<0)
#define CCER_CC2E	(1U<<4)

#define CC1S	(1U<<0)
#define CC2S	(1U<<8)
#define DIER_CC1IE	(1U<<1)
#define DIER_CC2IE	(1U<<2)
#define DIER_UIE	(1U<<0)
#define CC2P	(1U<<5)

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



//void tim2_pa5_output_compare(void)
//{
//	/*Enable Clock Access*/
//	RCC->AHB1ENR |= (1U<<0);
//
//	/*Set to alternate function mode*/
//	GPIOA->MODER |= (1U<<11);
//	GPIOA->MODER &=~(1U<<10);
//
//	/*Set alternate function type to AF01*/
//	GPIOA->AFR[0] &=~(1U<<23);
//	GPIOA->AFR[0] &=~(1U<<22);
//	GPIOA->AFR[0] &=~(1U<<21);
//	GPIOA->AFR[0] |= (1U<<20);
//
//	/*Enable clock access to tim2*/
//	RCC->APB1ENR |= TIM2EN;
//
//	/*Set the prescaler value*/
//	TIM2->PSC = 1600 - 1;
//	/*Set auto-reload value*/
//	TIM2->ARR = 10000 - 1;
//
//	/*Set output compare toggle mode*/
//	TIM2->CCMR1 = OC_TOGGLE;
//	/*Enable tim2 ch1 in compare mode*/
//	TIM2->CCER |= CCER_CC1E;
//
//
//	/*Clear counter*/
//	TIM2->CNT = 0;
//	/*Enable timer*/
//	TIM2->CR1 = CR1_CEN;
//}


void tim3_pa6_input_capture(void)
{
	/*Enable Clock Access*/
	RCC->AHB1ENR |= (1U<<0);

	/*Set to alternate function mode*/
	GPIOA->MODER |= (1U<<13);
	GPIOA->MODER &=~(1U<<12);

	/*Set alternate function type to AF01*/
	GPIOA->AFR[0] &=~(1U<<27);
	GPIOA->AFR[0] &=~(1U<<26);
	GPIOA->AFR[0] |= (1U<<25);
	GPIOA->AFR[0] &=~(1U<<24);

	/*Enable clock access to tim3*/
	RCC->APB1ENR |= TIM3EN;

	/*Set the prescaler value*/
	TIM3->PSC = 16 - 1;

	/*Setting the auto reload value*/
	TIM3->ARR = 0xFFFF;

	/*Set CH1 to input capture mode*/
	TIM3->CCMR1 = CC1S;

	/*Set CH1 to capture at rising edge*/
	TIM3->CCER = 0x0000;

	/*Enable Capture*/
	TIM3->CCER |= CCER_CC1E;

}

void tim3_pa7_input_capture(void)
{
	/*Enable Clock Access*/
	RCC->AHB1ENR |= (1U<<0);

	/*Set to alternate function mode*/
	GPIOA->MODER |= (1U<<15);
	GPIOA->MODER &=~(1U<<14);

	/*Set alternate function type to AF01*/
	GPIOA->AFR[0] &=~(1U<<31);
	GPIOA->AFR[0] &=~(1U<<30);
	GPIOA->AFR[0] |= (1U<<29);
	GPIOA->AFR[0] &=~(1U<<28);

	/*Enable clock access to tim3*/
	/*RCC->APB1ENR |= TIM3EN;*/

	/*Set the prescaler value*/
	/*TIM3->PSC = 16000 - 1;*/

	/*Setting the auto reload value*/
	/*TIM3->ARR = 0xFFFF;*/
	/*Set CH1 to input capture mode*/
	TIM3->CCMR1 |= CC2S;

	/*Set CH1 to capture at falling edge*/
	TIM3->CCER |= CC2P;

	/*Enable Capture*/
	TIM3->CCER |= CCER_CC2E;

	/*Enable timer*/
	TIM3->CR1 |= CR1_CEN;

	/*Enable TIM Interrupt*/
	TIM3->DIER |= (DIER_UIE | DIER_CC1IE | DIER_CC2IE);
	/*Enable TIM Interrupt in NVIC*/
	NVIC_EnableIRQ(TIM3_IRQn);
}



