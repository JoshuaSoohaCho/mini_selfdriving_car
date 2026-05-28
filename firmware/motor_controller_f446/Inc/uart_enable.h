/*
 * uart_enable.h
 *
 *  Created on: May 27, 2026
 *      Author: joshu
 */

#ifndef UART_ENABLE_H_
#define UART_ENABLE_H_

/* INCLUDES: */
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

/* DEFINITIONS */
#define GPIOAEN 					(1U<<0)
#define UART2EN						(1U<<17)

#define CR1_TE						(1U<<3)
#define CR1_UE						(1U<<13)
#define SR_TXE						(1U<<7)

#define SYS_FREQ 					16000000

#define APB1_CLK					SYS_FREQ

#define UART_BAUDRATE				115200


/* FUNCTION PROTOTYPES */
void uart2_tx_init(void);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baudrate);
static uint16_t compute_uart_div(uint32_t periph_clk, uint32_t baudrate);
void uart2_write(int ch);
int __io_putchar(int ch);





#endif /* UART_ENABLE_H_ */
