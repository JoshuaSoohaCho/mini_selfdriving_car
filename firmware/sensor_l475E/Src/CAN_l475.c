/*
 * CAN_l475.c
 *
 *  Created on: Jun 2, 2026
 *      Author: joshu
 */

#include "CAN_l475.h"

void CAN_init(void) {
	// RX GPIO to pull up
	GPIOB->PUPDR &= ~(1 << 16); // Clear pull-up/pull-down for PB8
	GPIOB->PUPDR |= (1 << 17);  // Set pull-up for PB8

	// Set PB8 and PB9 to alternate function mode
	GPIOB->MODER &= ~(1 << 16); // Clear mode for PB8
	GPIOB->MODER |= (1 << 17);  // Set PB8 to alternate function mode

	GPIOB->MODER &= ~(1 << 18); // Clear mode for PB9
	GPIOB->MODER |= (1 << 19);  // Set PB9 to alternate function mode

	// Set alternate function to AF9 for CAN
	// PIN 8
	GPIOB->AFR[1] |= (1 << 0); // AF9 for PB8 (CAN RX)
	GPIOB->AFR[1] &= ~(1 << 1); // Clear AF9 for PB8
	GPIOB->AFR[1] &= ~(1 << 2); // Clear AF9 for PB8
	GPIOB->AFR[1] |= (1 << 3); // Set AF9 for PB8

	// PIN 9
	GPIOB->AFR[1] |= (1 << 4); // AF9 for PB9 (CAN TX)
	GPIOB->AFR[1] &= ~(1 << 5);
	GPIOB->AFR[1] &= ~(1 << 6);
	GPIOB->AFR[1] |= (1 << 7);

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

void CAN_send_test(uint8_t *data) { // uint8_t data
	while (!(CAN1->TSR & (TME0))); // Wait until a transmit mailbox is empty
	CAN1->sTxMailBox[0].TIR = (0x007 << 21); // Standard ID (0x007)
	CAN1->sTxMailBox[0].TDTR = 1; // Data length (1 byte)
	CAN1->sTxMailBox[0].TDLR = data[0] << 0; // Load data into the mailbox

	CAN1->sTxMailBox[0].TIR |= (TXRQ); // Request transmission
}

void CAN_send_message(CAN_Message *msg){
	while(!(CAN1->TSR & (TME0))); // Wait until a transmit mailbox is empty

	CAN1->sTxMailBox[0].TIR = (0x100 << 21); // Standard ID (0x100)

	CAN1->sTxMailBox[0].TDTR = 4; // Data length (4 bytes)

	CAN1->sTxMailBox[0].TDLR =		(msg->node_id << 0) |
							 	 	(msg->command << 8) |
									(msg->value << 16 ); // Load data into the mailbox

	CAN1->sTxMailBox[0].TIR |= (TXRQ); // Request transmission
}

int CAN_receive(uint8_t *data){
	if(CAN1->RF0R & FMP0){
		*data = CAN1->sFIFOMailBox[0].RDLR & 0xFF; // Read received data
		CAN1->RF0R |= (RFOM0); // Release FIFO 0
		return 1;
	}
	return 0;
}
