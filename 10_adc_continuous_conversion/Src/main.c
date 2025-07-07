#include <stdio.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "adxl345.h"

uint16_t sensor_value;
uint32_t last_beat_time = 0;
uint8_t pulse_detected = 0;
uint32_t bpm = 0;

int16_t x,y,z;
double xg,yg,zg;

extern uint8_t data_rec[6];


#define THRESHOLD 1500

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (volatile uint32_t j = 0; j < 1600; j++) {
            __NOP();
        }
    }
}

int main(void)
{
    uart2_tx_init();
    pa1_adc_init();
    start_conversion();
    systick_init();  // Sets up 1ms SysTick, defines millis()
	adxl_init();


    printf("System Initializing...\n\r");

    while (1)
    {

            uint16_t val = adc_read();
            sensor_value = val;

            uint32_t now = millis();

            if (val > THRESHOLD && pulse_detected == 0)
            {
                uint32_t diff = now - last_beat_time;

                if (diff > 300 && diff < 1500)
                {
                    bpm = 60000 / diff;
                    if((bpm >= 55) && (bpm <= 105))
                    {
                    	printf("BPM: %lu\n\r", bpm);
                    	printf("x: %d\n\r", x);
                        printf("y: %d\n\r", y);
                        printf("z: %d\n\r", z);
                    }
                }

                last_beat_time = now;
                pulse_detected = 1;
            }

            if (val < THRESHOLD)
            {
                pulse_detected = 0;
            }

            delay_ms(5);  // Replace systick delay with for-loop

    		adxl_read_values(DATA_START_ADDR);
    		x = ((data_rec[1]<<8)|data_rec[0]);
    		y = ((data_rec[3]<<8)|data_rec[2]);
    		z = ((data_rec[5]<<8)|data_rec[4]);

    		xg = x * 0.0078;
    		yg = y * 0.0078;
    		zg = z * 0.0078;

    }
}
