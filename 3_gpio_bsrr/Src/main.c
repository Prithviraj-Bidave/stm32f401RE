#include "stm32f4xx.h"


volatile uint16_t adc_val0, adc_val3;

void tim2_pwm_init(void)
{
    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure PA5 and PA1 to alternate function AF1 (TIM2) */
    GPIOA->MODER &= ~((3U << 10) | (3U << 2));  // Clear
    GPIOA->MODER |=  ((2U << 10) | (2U << 2));  // AF mode

    GPIOA->AFR[0] &= ~((0xF << 20) | (0xF << 4)); // Clear AFR
    GPIOA->AFR[0] |=  ((1U << 20) | (1U << 4));   // AF1 = TIM2

    /* Enable TIM2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* Configure timer */
    TIM2->PSC = 1600 - 1;      // Timer clock = 16 MHz / 1600 = 10 kHz
    TIM2->ARR = 10000 - 1;     // PWM frequency = 10kHz / 10000 = 1 Hz

    /* PWM mode 1 on CH1 and CH2 */
    TIM2->CCMR1 &= ~((7U << 4) | (7U << 12));
    TIM2->CCMR1 |= ((6U << 4) | (6U << 12));     // OC1M = OC2M = 110 PWM mode 1

    /* Enable preload for CCR registers */
    TIM2->CCMR1 |= (1U << 3) | (1U << 11);

    /* Enable output on CH1 and CH2 */
    TIM2->CCER |= (1U << 0) | (1U << 4);

    /* Set duty cycles */
    TIM2->CCR1 = (TIM2->ARR + 1) * 50 / 100;    // 50%
    TIM2->CCR2 = (TIM2->ARR + 1) * 75 / 100;    // 75%

    /* Enable counter */
    TIM2->CR1 |= TIM_CR1_CEN;
}

void adc_init(void)
{
    /* Enable ADC1 and GPIOA clocks */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure PA0 and PA3 as analog */
    GPIOA->MODER |= (3U << 0) | (3U << 6);  // PA0, PA3 analog mode

    /* ADC Common config */
    ADC->CCR = 0; // prescaler

    /* ADC1 regular sequence: 2 conversions */
    ADC1->SQR1 = (1 << 20); // 2 conversions (L=1 means 2 conversions total)
    ADC1->SQR3 = (0 << 0) | (3 << 5); // 1st: channel0 (PA0), 2nd: channel3 (PA3)

    /* Sample time */
    ADC1->SMPR2 |= (3U << 0) | (3U << 9); // channels 0 and 3 sample time 56 cycles

    /* Enable ADC */
    ADC1->CR2 |= ADC_CR2_ADON;
}

void adc_start_conversion(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;  // start conversion
}

uint16_t adc_read_channel(uint8_t ch)
{
    // Start single conversion on channel ch
    ADC1->SQR3 = ch;      // Set channel
    ADC1->SQR1 = 0;       // One conversion

    // Start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for conversion complete
    while (!(ADC1->SR & ADC_SR_EOC)) {}

    // Read result
    return ADC1->DR;
}

int main(void)
{
    tim2_pwm_init();
    adc_init();

    while(1)
    {
        // Read ADC channel 0 (PA0)
        adc_val0 = adc_read_channel(0);

        // Read ADC channel 3 (PA3)
        adc_val3 = adc_read_channel(3);

        // Use adc_val0 and adc_val3 as needed...
        // (For debugging: send over UART, toggle LEDs, etc.)
    }
}
