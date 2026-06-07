/*
 * CAN_f446re.c
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

#include "CAN_f446re.h"



void CAN_init(void) {
	// RX GPIO to pull up
	GPIOA->PUPDR |= (1 << 22); // Set pull-up for PA11
	GPIOA->PUPDR &= ~(1 << 23);

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

	CAN1->MCR &= ~NART; // Disable automatic retransmission
	CAN1->MCR |= NART; // Enable automatic retransmission

	CAN1->BTR =
		(3 << 24) |   // SJW = 4
		(2 << 20) |   // TS2 = 3
		(9 << 16) |   // TS1 = 10
		(1 << 0);     // BRP = 2 // 500 kbps @ 16 MHz (known good)

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode

	while (CAN1->MSR & CAN_INRQ); // Wait until initialization mode is exited



	CAN1->FMR |= (FINIT); // Enter filter initialization mode

	CAN1->FA1R &= ~(FACT0); // Deactivate filter 0

	CAN1->FS1R |= FSC0; // Set filter 0 to mask mode // FSC0

	CAN1->sFilterRegister[0].FR1 = 0x00000000; // Filter ID (accept all)
	CAN1->sFilterRegister[0].FR2 = 0x00000000; // Filter mask (accept all)

	CAN1->FFA1R &= ~FFA0; // Activate filter 0 // FFA0

	CAN1->FA1R |= FACT0; // Activate filter 0

	CAN1->FMR &= ~(FINIT); // Exit filter initialization mode
	printf("CAN Complete\r\n");
}

void CAN_start(void) {
	CAN1->MCR &= ~SLEEP; // Ensure we're not in sleep mode
	CAN1->MCR &= ~CAN_INRQ; // Ensure we're not in initialization mode
	while (CAN1->MSR & SLAK); // Wait until not in sleep mode
	while (CAN1->MSR & INAK); // Wait until not in initialization mode
}

void CAN_loopback(void) {
	CAN1->MCR |= CAN_INRQ; // Enter initialization mode
	while (!(CAN1->MSR & INAK)); // Wait until initialization mode is entered

	CAN1->BTR |= LBKM; // Set LBKM bit for loopback mode

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode
	while (CAN1->MSR & INAK); // Wait until initialization mode is exited
}

void CAN_loopback_off(void) {
	CAN1->MCR |= CAN_INRQ; // Enter initialization mode
	while (!(CAN1->MSR & INAK)); // Wait until initialization mode is entered

	CAN1->BTR &= ~LBKM; // Clear LBKM bit to disable loopback mode

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode
	while (CAN1->MSR & INAK); // Wait until initialization mode is exited
}


int CAN_receive(uint8_t *data)
{
	if(CAN1->RF0R & FMP0){
		*data = CAN1->sFIFOMailBox[0].RDLR & 0xFF; // Read received data
		CAN1->RF0R |= (RFOM0); // Release FIFO 0
		return 1;
	}
	return 0;
}

void CAN_send(uint8_t *data) {	while (!(CAN1->TSR & (TME0))); // Wait until a transmit mailbox is empty
	CAN1->sTxMailBox[0].TIR = (0x007 << 21); // Standard ID (0x007)
	CAN1->sTxMailBox[0].TDTR = 1; // Data length (1 byte)
	CAN1->sTxMailBox[0].TDLR = data[0] << 0; // Load data into the mailbox

	CAN1->sTxMailBox[0].TIR |= (TXRQ); // Request transmission
	printf("ESR: 0x%08X\n\r", CAN1->ESR);}
