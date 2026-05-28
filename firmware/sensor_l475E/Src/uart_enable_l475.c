/*
 * uart_enable.c
 *
 *  Created on: May 28, 2026
 *      Author: joshu
 */

#include <uart_enable_l475.h>



void uart1_write(int ch) {
	/* Make sure transmit data register is empty */
	while (!(USART1->ISR & ISR_TXE));

	/* Write to transmit data register */
	USART1->TDR = (ch & 0xFF);
}

int __io_putchar(int ch) {
	uart1_write(ch);
	return ch;
}

void uart1_tx_init(void) {

	/* Enable Clock Access to GPIOB */
	RCC->AHB2ENR |= GPIOBEN;

	/* PB6 AF Mode */
	GPIOB->MODER &= ~(1U << 12);
	GPIOB->MODER |= (1U << 13);


	/* AF7 for USART1 */
	GPIOB->AFR[0] |= (1U << 24);
	GPIOB->AFR[0] |= (1U << 25);
	GPIOB->AFR[0] |= (1U << 26);
	GPIOB->AFR[0] &= ~(1U << 27);

	/* Enable Clock Access to UART2 */
	RCC->APB2ENR |= UART1EN;

	/* Configure Baudrate */
	uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);
//	USART1->BRR = (0x1A0);

	/* Configure transfer direction */
	USART1->CR1 = CR1_TE;

	/* Enable the USART2 Transmitter */
	USART1->CR1 |= CR1_UE;

}

static uint16_t compute_uart_div(uint32_t periph_clk, uint32_t baudrate) {	return ((periph_clk + (baudrate / 2U)) / baudrate);}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baudrate) {	USARTx->BRR = compute_uart_div(periph_clk, baudrate);}

int main() {
	uart1_tx_init();

	while(1){
		printf("Hello World!\n\r");
		for(int i = 0; i < 100000; i++){}
	}
}

