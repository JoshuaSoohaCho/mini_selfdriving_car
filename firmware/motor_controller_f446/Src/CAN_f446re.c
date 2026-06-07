/*
 * CAN_f446re.c
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

#include "CAN_f446re.h"



void CAN_init(void) {
	// Enable clock for CAN peripheral
	RCC->APB1ENR |= CAN1EN;

//	// Configure CAN pins (e.g., PA11 for RX, PA12 for TX)
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable clock for GPIOA

	// Set PA11 and PA12 to alternate function mode
	GPIOA->MODER &= ~(1 << 22);
	GPIOA->MODER |= (1 << 23);

	GPIOA->MODER &= ~(1 << 24);
	GPIOA->MODER |= (1 << 25);

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


//	CAN1->FMR |= (FINIT); // Enter filter initialization mode
//
//	CAN1->FA1R &= ~(CAM1_FA1R_FACT0); // Deactivate filter 0
//
//	CAN1->FMR &= ~(FINIT); // Exit filter initialization mode

	/* Enter filter init mode */
	CAN1->FMR |= (1U << 0);

	/* Deactivate filter 0 */
	CAN1->FA1R &= ~(1U << 0);

	/* Set filter to accept ALL messages */
	CAN1->FS1R |= (1U << 0);   // 32-bit mode
	CAN1->FM1R &= ~(1U << 0);  // mask mode

	CAN1->sFilterRegister[0].FR1 = 0x00000000;
	CAN1->sFilterRegister[0].FR2 = 0x00000000;

	/* Assign filter to FIFO 0 */
	CAN1->FFA1R &= ~(1U << 0);

	/* Activate filter */
	CAN1->FA1R |= (1U << 0);

	/* Leave filter init mode */
	CAN1->FMR &= ~(1U << 0);
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

	CAN1->BTR |= (1 << 30); // Set LBKM bit for loopback mode

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode
	while (CAN1->MSR & INAK); // Wait until initialization mode is exited
}

void CAN_loopback_off(void) {
	CAN1->MCR |= CAN_INRQ; // Enter initialization mode
	while (!(CAN1->MSR & INAK)); // Wait until initialization mode is entered

	CAN1->BTR &= ~(1 << 30); // Clear LBKM bit to disable loopback mode

	CAN1->MCR &= ~CAN_INRQ; // Exit initialization mode
	while (CAN1->MSR & INAK); // Wait until initialization mode is exited
}


uint8_t CAN_receive(void)
{
	CAN1->MCR &= ~SLEEP;
	CAN1->MCR &= ~CAN_INRQ; // Ensure we're not in initialization mode
	while(CAN1->MSR & SLAK); // Wait until not in sleep mode
	while(CAN1->MSR & INAK); // Wait until not in initialization mode

    /* Wait for message */
	printf("[CAN] Waiting for message in FIFO 0...\n\r");
    while (!(CAN1->RF0R & (1U << 0)));
    printf("[CAN] Message received in FIFO 0\n\r");

    /* Read data */
    uint8_t data = CAN1->sFIFOMailBox[0].RDLR & 0xFF;

    /* Release FIFO */
    CAN1->RF0R |= (1U << 5);

    return data;
}

void CAN_send(uint8_t *data) {	CAN1->MCR &= ~SLEEP;	CAN1->MCR &= ~CAN_INRQ; // Ensure we're not in initialization mode	while(CAN1->MSR & SLAK); // Wait until not in sleep mode	while(CAN1->MSR & INAK); // Wait until not in initialization mode	CAN1->sTxMailBox[0].TIR = 0x00000000; // Standard ID, no RTR	CAN1->sTxMailBox[0].TDTR = 1; // Data length = 1 byte	CAN1->sTxMailBox[0].TDLR = *data; // Load data into mailbox	CAN1->sTxMailBox[0].TIR |= (1U << 0); // Request transmission}
