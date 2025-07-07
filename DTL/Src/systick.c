#include "stm32f4xx.h"

volatile uint32_t tick = 0;

void systick_init(void) {
    SysTick_Config(16000);  // 16 MHz → 1 ms tick
}

uint32_t millis(void) {
    return tick;
}

void SysTick_Handler(void) {
    tick++;
}
