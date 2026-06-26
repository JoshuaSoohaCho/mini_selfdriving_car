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

	// Enable clock for GPIOC (used for i2c)
	RCC->AHB2ENR |= GPIOCEN;

	// Enable CLock for CAN1
	RCC->APB1ENR1 |= CAN1EN;

	// Enable Clock for I2C2
	RCC->APB1ENR1 |= I2C3EN;

	// Additional system initialization code can be added here if needed
	RCC->CR |= HSION;              // Enable HSI
	while (!(RCC->CR & HSIRDY));   // Wait ready

	RCC->CFGR |= HSI16_SEL;           // Select HSI as SYSCLK
}

