#include "exti.h"

#define SYSCFGEN		(1U<<14)



void pc13_exti_init(void)
{
	/*Disable Global Interrupts*/
	__disable_irq();

	/*Enable Clock access for GPIOC*/
	RCC->AHB1ENR |= (1U<<2);
	/*Enable clock access to SYSCFG*/
	RCC->APB2ENR |= SYSCFGEN;
	/*Set PC13 as input*/
	GPIOC->MODER &=~(1U<<27);
	GPIOC->MODER &=~(1U<<26);

	/*Select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] |= (1U<<5);

	/*Unmask EXTI13*/
	EXTI->IMR |= (1U<<13);

	/*Select falling edge trigger*/
	EXTI->FTSR |= (1U<<13);

	/*Enable EXTI13 line in NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/*Enable Global Interrupts*/
	__enable_irq();

}
