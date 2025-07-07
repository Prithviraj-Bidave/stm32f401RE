#include "stm32f4xx.h"


#define GPIOBEN		(1U<<1)
#define I2C1EN		(1U<<21)

#define I2C_100KHz	80
#define SD_MODE_MAX_RISE_TIME		17

#define CR1_PE		(1U<<0)

#define SR2_BUSY	(1U<<1)
#define CR1_START	(1U<<8)
#define SR1_SB		(1U<<0)
#define SR1_ADDR	(1U<<1)
#define SR1_TXE		(1U<<7)
#define CR1_ACK		(1U<<10)
#define CR1_STOP	(1U<<9)
#define SR1_RXNE	(1U<<6)
#define SR1_BTF		(1U<<2)
/*Pinout
 * PB8 ---- SCL
 * PB9 ---- SDA
 */
void I2C1_init(void)
{
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= GPIOBEN;
	/*Set PB8 and PB9 mode to alternate function*/
	GPIOB->MODER &=~(1U<<16);
	GPIOB->MODER |= (1U<<17);
	GPIOB->MODER &=~(1U<<18);
	GPIOB->MODER |= (1U<<19);
	/*Set PB8 and PB9 output type to open drain*/
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);
	/*Enable pull up for PB8 and PB9*/
	GPIOB->PUPDR &=~(1U<<17);
	GPIOB->PUPDR |= (1U<<16);

	GPIOB->PUPDR &=~(1U<<19);
	GPIOB->PUPDR |= (1U<<18);

	/*Set PB8 and PB9 alternate function type to I2C(AF4)*/
	GPIOB->AFR[1] &=~(1U<<0);
	GPIOB->AFR[1] &=~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &=~(1U<<3);

	GPIOB->AFR[1] &=~(1U<<4);
	GPIOB->AFR[1] &=~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &=~(1U<<7);

		/*Enable clock access to I2C1*/
	RCC->APB1ENR |= I2C1EN;
	/*Enter reset mode*/
	I2C1->CR1 |= (1U<<5);
	/*EXIT reset mode*/
	I2C1->CR1 &=~(1U<<5);
	/*Set Peripheral Frequency*/
	I2C1->CR2 |= (1U<<4);
	/*Set I2C to standard mode, 100khz clock*/
	I2C1->CCR = I2C_100KHz;
	/*Set rise time*/
	I2C1->TRISE = I2C_100KHz;
	/*Enable I2C1 module*/
	I2C1->CR1 |= CR1_PE;

}


void I2C1_byteRead(char saddr, char maddr, char* data)
{
	volatile int tmp;
	/*wait until bus not busy*/
	while(I2C1->SR2 & (SR2_BUSY));
	/*Generate Start Condition*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & (SR1_SB)));
	/* Transmit slave address + Write */
	I2C1->DR = saddr << 1;
	/*Wait until addr flag is set*/
	while (!(I2C1->SR1 & (SR1_ADDR)));
	/*clear addr flag*/
	tmp = I2C1->SR2;
	/*Send memory address*/
	I2C1->DR = maddr;
	/*Wait until transmitter ready*/
	while(!(I2C1->SR1 & SR1_TXE));

	/*Generate restart Condition*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & (SR1_SB)));
	/*Transmit slave address + Read*/
	I2C1->DR = saddr << 1|1 ;
	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR)));
	/*Disable Acknowledge*/
	I2C1->CR1 &=~CR1_ACK;
	/*clear addr flag*/
	tmp = I2C1->SR2;
	/*Generate Stop after data received*/
	I2C1->CR1 |= CR1_STOP;
	/*Wait until RXNE flag is set*/
	while(!(I2C1->SR1 & SR1_RXNE));

	/*Read from data register*/
	*data++ = I2C1->DR;
}

void I2C1_burstREAD(char saddr, char maddr, int n, char* data)
{
	volatile int tmp;
	/*wait until bus not busy*/
	while(I2C1->SR2 & (SR2_BUSY));
	/*Generate Start Condition*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & (SR1_SB)));
	/*Transmit slave address + Write*/
	I2C1->DR = saddr << 1;
	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR)));
	/*clear addr flag*/
	tmp = I2C1->SR2;
	/*Wait until transmitter ready*/
	while(!(I2C1->SR1 & SR1_TXE));
	/*Send memory address*/
	I2C1->DR = maddr;
	/*Wait until transmitter ready*/
	while(!(I2C1->SR1 & SR1_TXE));
	/*Generate restart Condition*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & (SR1_SB)));
	/*Transmit slave address + Read*/
	I2C1->DR = saddr << 1|1 ;
	/*clear addr flag*/
	tmp = I2C1->SR2;
	/*Enable Acknowledge*/
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U)
	{
		/*if one byte*/
		if(n == 1U)
		{
			/*Disable Acknowledge*/
			I2C1->CR1 &=~CR1_ACK;
			/*Generate Stop after data received*/
			I2C1->CR1 |= CR1_STOP;
			/*Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & SR1_RXNE));
			/*Read from data register*/
			*data++ = I2C1->DR;
			break;
		}
		else
		{
			/*Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & SR1_RXNE));
			/*Read from data register*/
			(*data++) = I2C1->DR;
			n--;
		}
	}
}

void I2C1_burstWrite(char saddr, char maddr, int n, char *data)
{
	volatile int tmp;
	/*wait until bus not busy*/
	while(I2C1->SR2 & (SR2_BUSY));
	/*Generate Start Condition*/
	I2C1->CR1 |= CR1_START;
	/*Wait until start flag is set*/
	while(!(I2C1->SR1 & (SR1_SB)));
	/*Transmit slave address + Write*/
	I2C1->DR = saddr << 1;
	/*Wait until addr flag is set*/
	while(!(I2C1->SR1 & (SR1_ADDR)));
	/*clear addr flag*/
	tmp = I2C1->SR2;
	/*Wait until data register empty*/
	while(!(I2C1->SR1 & SR1_TXE));
	/*Send memory address*/
	I2C1->DR = maddr;
	for(int i = 0; i < n; i++)
	{
		/*Wait until data register empty*/
		while(!(I2C1->SR1 & SR1_TXE));
		/*Send memory address*/
		I2C1->DR = *data++;
	}
	while(!(I2C1->SR1 & SR1_BTF));
	/*Generate Stop Condition*/
	I2C1->CR1 |= CR1_STOP;
}
