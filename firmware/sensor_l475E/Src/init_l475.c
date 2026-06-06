/*
 * init_l475.c
 *
 *  Created on: Jun 4, 2026
 *      Author: joshu
 */
// Shared Initialization Code for L475

#include "init_l475.h"


void System_Init(void) {
	// Enable clock for GPIOB (used for UART and CAN)
	RCC->AHB2ENR |= GPIOBEN;

	// Additional system initialization code can be added here if needed
	RCC->CR |= (1 << 8);              // Enable HSI
	while (!(RCC->CR & (1 << 10)));   // Wait ready

	RCC->CFGR |= (1 << 0);           // Select HSI as SYSCLK
}

