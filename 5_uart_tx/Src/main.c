#include "stm32f4xx.h"
#include <stdint.h>

#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	115200

#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_tx_init(void);
void uart2_write(int ch);



int main(void)
{
	uart2_tx_init();
	while(1)
	{
		uart2_write('Y');
		for(int i = 0; i < 1000000; i++);
		uart2_write(' ');
	}
}


void uart2_tx_init(void)
{
	/******Configure UART gpio pins******/
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~(1U<<4);

	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<8);


	/******Configure UART module******/
	/*Enable clock access to uart2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART2->CR1 = CR1_TE;

	/*Enable UART module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE));
	/*Write to the transmit data register*/
	USART2->DR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
