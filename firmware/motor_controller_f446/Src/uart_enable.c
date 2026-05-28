/*
 * uart_enable.c
 * For STM32F4 Discovery Board
 *  Created on: May 27, 2026
 *      Author: joshu
 */



#include <uart_enable.h>


void uart2_tx_init(void){
	/* Enable Clock Access to GPIO_A */
	RCC->AHB1ENR |= GPIOAEN;

	/* PA2 AF Mode */
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);

	/* AF7 for USART2 */
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] &= ~(1U << 11);

	/* Enable Clock Access to USART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure Baudrate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure transfer direction */
	USART2->CR1 = CR1_TE;

	/* Enable the USART2 Transmitter */
	USART2->CR1 |= CR1_UE;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baudrate){
	USARTx->BRR = compute_uart_div(periph_clk, baudrate);
}

static uint16_t compute_uart_div(uint32_t periph_clk, uint32_t baudrate){
	return ((periph_clk + (baudrate/2U)) / baudrate);
}

void uart2_write(int ch){
	/* Make sure transmit data register is empty */
	while (!(USART2->SR & SR_TXE));

	/* Write to transmit data register */
	USART2->DR = (ch & 0xFF);
}


int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}



int main(){
	uart2_tx_init();

	while(1){
		printf("Hello World!\n\r");
		for(int i = 0; i < 100000; i++){}
	}
}

