/*
 * CAN_l475.c
 *
 *  Created on: Jun 2, 2026
 *      Author: joshu
 */

#include "CAN_l475.h"

void CAN_init(void) {
	// Enable clock for CAN peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_CAN1EN;

	// Configure CAN pins (e.g., PA11 for RX, PA12 for TX)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable clock for GPIOA

	// Set PA11 and PA12 to alternate function mode
	GPIOA->MODER &= ~(1 << 22);
	GPIOA->MODER |= (1 << 23);

	GPIOA->MODER &= ~(1 << 24);
	GPIOA->MODER |= (1 << 25);

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

	// Additional CAN configuration would go here (e.g., bit timing, filters)
	CAN1->MCR |= CAN_INRQ; // Enter initialization mode

	CAN1->BTR = (4 << 20) | (3 << 16) | (1 << 0); // Example bit timing configuration (adjust as needed)

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode
	while (CAN1->MSR & CAN_INRQ); // Wait until initialization mode is exited

	CAN1->FMR |= (CAN_FMR_FINIT); // Enter filter initialization mode

	CAN1->FA1R &= ~(CAM1_FA1R_FACT0); // Deactivate filter 0

	CAN1->FMR &= ~(CAN_FMR_FINIT); // Exit filter initialization mode
}


void CAN_send(uint8_t data) {
	while (!(CAN1->TSR & (1U << 26))); // Wait until a transmit mailbox is empty

	CAN1->sTxMailBox[0].TIR = (0x100 << 21); // Standard ID (0x100)

	CAN1->sTxMailBox[0].TDTR = 1; // Data length (1 byte)

	CAN1->sTxMailBox[0].TDLR = data; // Load data into the mailbox

	CAN1->sTxMailBox[0].TIR |= (1U << 0); // Request transmission
}
