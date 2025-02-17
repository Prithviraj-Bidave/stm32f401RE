#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include <stdbool.h>



bool is_start = false;
uint32_t value_1, value_2;

uint16_t count = 0;
uint16_t dist_cm;

int main(void)
{
    // Initialize peripherals
    tim3_pa6_input_capture();
    tim3_pa7_input_capture();
    uart2_tx_init();

    // Configure PA5 as output
    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);

    // Enable TIM3 interrupt in NVIC
    NVIC_EnableIRQ(TIM3_IRQn);

    printf("System Initializing...\n\r");

    // Main loop
    while (1)
    {
        GPIOA->ODR |= (1U << 5);      // Send trigger pulse
        systickDelayMicroSec(10);    // 10us delay
        GPIOA->ODR &= ~(1U << 5);    // End trigger pulse
        systickDelayMs(5000);        // 5s delay
    }
}

void TIM3_IRQHandler(void)
{
    // Handle CC1 interrupt (rising edge)
    if (TIM3->SR & SR_CC1IF)
    {
        value_1 = count * (TIM3->CCR1); /* Read captured value */
        TIM3->SR &= ~SR_CC1IF;         /* Clear CC1IF flag */
        printf("CC1 Triggered: %ld\n\r", value_1);
    }

    // Handle CC2 interrupt (falling edge)
    if (TIM3->SR & SR_CC2IF)
    {
        value_2 = count * (TIM3->CCR2); /* Read captured value */
        TIM3->SR &= ~SR_CC2IF;         /* Clear CC2IF flag */
//        dist_cm = (uint16_t)(((float)(value_2 - value_1) * 1.715f) / 2.0f);
//        printf("Distance: %u cm\n\r", dist_cm);
        count = 0; // Reset overflow count
    }

    // Handle timer overflow interrupt
    if (TIM3->SR & SR_UIF)
    {
        count++;
        TIM3->SR &= ~SR_UIF; // Clear update interrupt flag
    }
}



void HardFault_Handler(void)
{
	while (1)
	    {
	        printf("HardFault occurred\n\r");
	    }
}


void UsageFault_Handler(void)
{
    while (1)
    {
        printf("UsageFault occurred\n\r");
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
        printf("BusFault occurred\n\r");
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
        printf("Memory Management Fault occurred\n\r");
    }
}

