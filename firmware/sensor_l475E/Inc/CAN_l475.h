/*
 * CAN_l475.h
 *
 *  Created on: Jun 2, 2026
 *      Author: joshu
 */

#ifndef CAN_L475_H_
#define CAN_L475_H_


/* Includes */
#include "stm32l475xx.h"
#include "main.h"


/* Definitions */
// APB1 peripheral clock enable register 1 (RCC_APB1ENR1) bits
#define CAN1EN			(1U << 25)  // CAN1 clock enable

// CAN_MCR bits
#define CAN_INRQ			(1U << 0)  // Initialization Request
#define NART				(1U << 4)  // No Automatic Retransmission
#define SLEEP				(1U << 1)  // Sleep Mode

// CAN_FMR bits
#define FINIT				(1U << 0)  // Filter Initialization Mode

// CAN_FS1R bits
#define FSC0				(1U << 0)  // Filter 0 scale configuration bit (0 = 16-bit, 1 = 32-bit)

// CAN_FFA1R bits
#define FFA0				(1U << 0)  // Filter 0 FIFO assignment bit (0 = FIFO 0, 1 = FIFO 1)

// CAN_FA1R bits
#define FACT0				(1U << 0)  // Filter 0 activation bit

// CAN_MSR bits
#define SLAK				(1U << 1)  // Sleep Mode Request
#define INAK				(1U << 0)  // Initialization Acknowledge

// CAN_BTR bits
#define LBKM				(1U << 30) // Loopback Mode bit in CAN_BTR

// CAN_TSR bits
#define TME0				(1U << 26) // Transmit Mailbox Empty bit for mailbox 0

// CAN sTxMailBox TIR Bits
#define TXRQ				(1U << 0)  // Transmit Request bit in TIR

// CAN_RF0R bits
#define FMP0				(1U << 0)  // FIFO 0 message pending bit
#define RFOM0				(1U << 5)  // Release FIFO 0 output mailbox bit

/* Function prototypes */
void CAN_init(void);
void CAN_start(void);
void CAN_loopback(void);

void CAN_send(uint8_t *data);
int CAN_receive(uint8_t *data);


#endif /* CAN_L475_H_ */
