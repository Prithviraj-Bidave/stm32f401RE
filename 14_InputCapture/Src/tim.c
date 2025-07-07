#include "stm32f4xx.h"



#define TIM2EN		(1U<<0)
#define TIM3EN		(1U<<1)

#define CR1_CEN		(1U<<0)
#define OC_TOGGLE	((1U<<4) | (1U<<5))
#define CCER_CC1E	(1U<<0)

#define CC1S	(1U<<0)

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
	TIM2->PSC = 1600 - 1;
	/*Set auto-reload value*/
	TIM2->ARR = 10000 - 1;

	/*Set output compare toggle mode*/
	TIM2->CCMR1 = OC_TOGGLE;
	/*Enable tim2 ch1 in compare mode*/
	TIM2->CCER |= CCER_CC1E;


	/*Clear counter*/
	TIM2->CNT = 0;
	/*Enable timer*/
	TIM2->CR1 = CR1_CEN;
}


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
	TIM3->PSC = 16000 - 1;

	/*Set CH1 to input capture mode*/
	TIM3->CCMR1 = CC1S;

	/*Set CH1 to capture at rising edge*/
	TIM3->CCER = CCER_CC1E;

	/* Set trigger selection to TI1FP1 (filtered TI1) */
    TIM3->SMCR &= ~(0x7 << 4);
    TIM3->SMCR |= (0x5 << 4); // TS = 101: TI1FP1

    /* Set slave mode to reset mode */
    TIM3->SMCR &= ~(0x7);
    TIM3->SMCR |= (0x4); // SMS = 100: Reset mode

	/*Enable timer*/
	TIM3->CR1 = CR1_CEN;
}


void tim3_pa6_pa7_input_capture(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= (1U << 0);

    /* Configure PA6 (TIM3_CH1) to alternate function AF2 */
    GPIOA->MODER &= ~(3U << 12);    // Clear MODER bits for PA6 (bits 12-13)
    GPIOA->MODER |=  (2U << 12);    // Set AF mode (10) for PA6
    GPIOA->AFR[0] &= ~(0xF << 24);  // Clear AFR bits for PA6 (bits 24-27)
    GPIOA->AFR[0] |=  (2U << 24);   // Set AF2 for PA6

    /* Configure PA7 (TIM3_CH2) to alternate function AF2 */
    GPIOA->MODER &= ~(3U << 14);    // Clear MODER bits for PA7 (bits 14-15)
    GPIOA->MODER |=  (2U << 14);    // Set AF mode (10) for PA7
    GPIOA->AFR[0] &= ~(0xF << 28);  // Clear AFR bits for PA7 (bits 28-31)
    GPIOA->AFR[0] |=  (2U << 28);   // Set AF2 for PA7

    /* Enable TIM3 clock */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    /* Set prescaler */
    TIM3->PSC = 16000 - 1;

    /* Configure CH1 (PA6) for input capture on TI1 */
    TIM3->CCMR1 &= ~(0x3);          // Clear CC1S bits
    TIM3->CCMR1 |=  (0x1 << 0);     // CC1S = 01 (TI1)

    /* Configure CH2 (PA7) for input capture on TI2 */
    TIM3->CCMR1 &= ~(0x3 << 8);     // Clear CC2S bits
    TIM3->CCMR1 |=  (0x1 << 8);     // CC2S = 01 (TI2)

    /* Capture on rising edge for both channels */
    TIM3->CCER &= ~((1U << 1) | (1U << 5)); // CC1P = 0, CC2P = 0 (rising edge)
    TIM3->CCER |=  ((1U << 0) | (1U << 4)); // CC1E = 1, CC2E = 1 (enable capture)

    /* Set trigger to filtered timer input 1 and 2 */
    // For simplicity, use TI1FP1 (TS=101) for both? Actually only one trigger source allowed.
    // Typically, the timer reset slave mode can only be triggered by one trigger source.
    // So this setup is not straightforward for simultaneous independent capture reset.
    // You may want to handle this in software or by configuring two timers.

    // But if you want reset on TI1FP1:
    TIM3->SMCR &= ~(0x7 << 4);
    TIM3->SMCR |=  (0x5 << 4);      // TS = 101: TI1FP1

    /* Set slave mode to reset mode */
    TIM3->SMCR &= ~(0x7);
    TIM3->SMCR |=  (0x4);           // SMS = 100: Reset mode

    /* Enable timer */
    TIM3->CR1 |= (1U << 0);
}

void tim2_pa5_pa1_output_compare(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= (1U << 0);

    /* Configure PA5 (TIM2_CH1) to AF1 */
    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (2U << 10);
    GPIOA->AFR[0] &= ~(0xF << 20);
    GPIOA->AFR[0] |=  (1U << 20);

    /* Configure PA1 (TIM2_CH2) to AF1 */
    GPIOA->MODER &= ~(3U << 2);
    GPIOA->MODER |=  (2U << 2);
    GPIOA->AFR[0] &= ~(0xF << 4);
    GPIOA->AFR[0] |=  (1U << 4);

    /* Enable TIM2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* Set prescaler */
    TIM2->PSC = 1600 - 1;

    /* Set auto-reload value */
    TIM2->ARR = 10000 - 1;

    /* Set toggle mode for CH1 (OC1M = 011) */
    TIM2->CCMR1 &= ~(0xFF);
    TIM2->CCMR1 |= (0x3 << 4);  // OC1M = toggle
    TIM2->CCMR1 &= ~(1 << 3);   // OC1PE = 0 (no preload)

    /* Set toggle mode for CH2 (OC2M = 011) */
    TIM2->CCMR1 &= ~(0xFF00);
    TIM2->CCMR1 |= (0x3 << 12); // OC2M = toggle
    TIM2->CCMR1 &= ~(1 << 11);  // OC2PE = 0

    /* Enable output for CH1 and CH2 */
    TIM2->CCER |= (1 << 0);  // CC1E
    TIM2->CCER |= (1 << 4);  // CC2E

    /* Set different compare values for toggle rates */
    TIM2->CCR1 = 7500;  // Toggle on count = 2500 (faster)
    TIM2->CCR2 = 2500;  // Toggle on count = 7500 (slower)

    /* Clear counter */
    TIM2->CNT = 0;

    /* Enable timer */
    TIM2->CR1 |= 1;
}
