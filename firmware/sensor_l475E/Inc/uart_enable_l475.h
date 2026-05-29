/*
 * uart_enable.h
 *
 *  Created on: May 28, 2026
 *      Author: joshu
 */

#ifndef UART_ENABLE_L475_H_
#define UART_ENABLE_L475_H_

/****************** INCLUDES ******************/
#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"

/****************** DEFINITIONS ******************/
#define GPIOBEN 					(1U<<1)
#define UART1EN						(1U<<14)

#define SYS_FREQ 					4000000 // 4 MHz
#define APB2_CLK					SYS_FREQ
#define UART_BAUDRATE				115200

#define CR1_TE						(1U<<3)
#define CR1_UE						(1U<<0)

#define ISR_TXE						(1U<<7)

/* Function Prototypes */
void uart1_tx_init(void);
static uint16_t compute_uart_div(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baudrate);
void uart2_write(int ch);
int __io_putchar(int ch);

#endif /* UART_ENABLE_L475_H_ */
