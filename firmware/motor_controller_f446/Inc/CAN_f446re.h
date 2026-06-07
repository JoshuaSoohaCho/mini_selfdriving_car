/*
 * CAN_f446re.h
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

#ifndef CAN_F446RE_H_
#define CAN_F446RE_H_

/* INCLUDES */
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

/* Definitions */

// CAN_MCR bits
#define CAN_INRQ			(1U << 0)  // Initialization Request
#define SLEEP				(1U << 1)  // Sleep Mode
#define NART				(1U << 4)  // No Automatic Retransmission

// CAN_MSR bits
#define INAK				(1U << 0)  // Initialization Acknowledge
#define SLAK				(1U << 1)  // Sleep Mode Request

// CAN_TSR bits
#define TME0				(1U << 26) // Transmit Mailbox Empty bit for mailbox 0

// CAN_RF0R bits
#define FMP0				(1U << 0)  // FIFO 0 message pending bit
#define RFOM0				(1U << 5)  // Release FIFO 0 output mailbox bit

// CAN_BTR bits
#define LBKM				(1U << 30) // Loopback Mode bit in CAN_BTR

// CAN sTxMailBox TIR Bits
#define TXRQ				(1U << 0)  // Transmit Request bit in TIR

// CAN_FMR bits
#define FINIT				(1U << 0)  // Filter Initialization Mode

// CAN_FS1R bits
#define FSC0				(1U << 0)  // Filter 0 scale configuration bit (0 = 16-bit, 1 = 32-bit)

// CAN_FFA1R bits
#define FFA0				(1U << 0)  // Filter 0 FIFO assignment bit (0 = FIFO 0, 1 = FIFO 1)

// CAN_FA1R bits
#define FACT0				(1U << 0)  // Filter 0 activation bit

/* FUNCTION PROTOTYPES */
void CAN_init(void);
int CAN_receive(uint8_t *data);
void CAN_start(void);
void CAN_loopback(void);
void CAN_loopback_off(void);
void CAN_send(uint8_t *data); // uint8_t data


#endif /* CAN_F446RE_H_ */
