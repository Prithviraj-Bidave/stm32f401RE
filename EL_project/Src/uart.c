#include "uart.h"

#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	115200

#define CR1_TE			(1U<<3)
#define CR1_RE			(1U<<2)
#define CR1_UE			(1U<<13)
#define CR1_RXNEIE		(1U<<5)
#define CR1_TXEIE		(1U<<7)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);
void uart6_write(int ch);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}


void uart2_rx_interrupt_init(void)
{
	/******Configure UART gpio pins******/
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~(1U<<4);

	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<8);

	GPIOA->MODER |= (1U<<7);
	GPIOA->MODER &=~(1U<<6);

	GPIOA->AFR[0] &=~(1U<<15);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<12);

	/******Configure UART module******/
	/*Enable clock access to uart2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE);
	/*Enable RXNE Interrupt*/
	USART2->CR1 |= CR1_RXNEIE;
	/*Enable UART2 interrupt in NVIC*/
	NVIC_EnableIRQ(USART2_IRQn);
	/*Enable UART module*/
	USART2->CR1 |= CR1_UE;
}



void uart2_rxtx_init(void)
{
	/******Configure UART gpio pins******/
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~(1U<<4);

	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<8);

	GPIOA->MODER |= (1U<<7);
	GPIOA->MODER &=~(1U<<6);

	GPIOA->AFR[0] &=~(1U<<15);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<12);

	/******Configure UART module******/
	/*Enable clock access to uart2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE);

	/*Enable UART module*/
	USART2->CR1 |= CR1_UE;
}



void uart6_tx_init(void)
{
	/******Configure UART gpio pins******/
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<23);
	GPIOA->MODER &=~(1U<<22);

	GPIOA->AFR[0] |=(1U<<15);
	GPIOA->AFR[0] &=~(1U<<14);
	GPIOA->AFR[0] &=~(1U<<13);
	GPIOA->AFR[0] &=~(1U<<12);



	/******Configure UART module******/
	/*Enable clock access to uart2*/
	RCC->APB2ENR |= (1U<<5);

	/*Configure baudrate*/
	uart_set_baudrate(USART6, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART6->CR1 = (CR1_RE);

	/*Enable UART module*/
	USART6->CR1 |= CR1_UE;
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

char uart2_read(void)
{
	/*Make sure the receive data register is not empty*/
	while(!(USART2->SR & SR_RXNE));
	/*Read the data register*/
	return USART2->DR;
}

void uart2_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE));
	/*Write to the transmit data register*/
	USART2->DR = (ch & 0xFF);
}

void uart6_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART6->SR & SR_TXE));
	/*Write to the transmit data register*/
	USART6->DR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}


