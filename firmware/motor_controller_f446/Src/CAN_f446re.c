/*
 * CAN_f446re.c
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

#include "CAN_f446re.h"



void CAN_init(void) {
	// Enable clock for CAN peripheral
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

//	// Configure CAN pins (e.g., PA11 for RX, PA12 for TX)
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable clock for GPIOA

	// Set PA11 and PA12 to alternate function mode
	GPIOA->MODER &= ~(1 << 22);
	GPIOA->MODER |= (1 << 23);

//	GPIOA->MODER &= ~((3U << (11 * 2)) | (3U << (12 * 2))); // Clear mode bits
//	GPIOA->MODER |= ((2U << (11 * 2)) | (2U << (12 * 2))); // Set to AF mode

	// Set alternate function to AF9 for CAN
	// PIN 11
	GPIOA->AFR[1] |= (1 << 12);
	GPIOA->AFR[1] &= ~(1 << 13);
	GPIOA->AFR[1] &= ~(1 << 14);
	GPIOA->AFR[1] |= (1 << 15);

	// PIN 12
	GPIOA->AFR[1] |= (1 << 16);
	GPIOA->AFR[1] &= ~(1 << 17);
	GPIOA->AFR[1] &= ~(1 << 18);
	GPIOA->AFR[1] |= (1 << 19);


//	GPIOA->AFR[1] |= (9U << ((11 - 8) * 4)) | (9U << ((12 - 8) * 4)); // AF9 for CAN

	// Additional CAN configuration would go here (e.g., bit timing, filters)
	CAN1->MCR |= CAN_INRQ; // Enter initialization mode

	CAN1->BTR = (4 << 20) | (3 << 16) | (1 << 0); // Example bit timing configuration (adjust as needed)

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode
	while (CAN1->MSR & CAN_INRQ); // Wait until initialization mode is exited
}


