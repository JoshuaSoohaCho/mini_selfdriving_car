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
#define CAN1EN			(1U << 25)  // CAN1 clock enable
#define GPIOBEN			(1U << 1)  // GPIOB clock enable

#define CAN_INRQ					(1U << 0)  // Initialization Request
#define FINIT				(1U << 0)  // Filter Initialization Mode
#define FACT0				(1U << 0)  // Filter 0 activation bit
#define NART				(1U << 4)  // No Automatic Retransmission
#define SLEEP				(1U << 1)  // Sleep Mode
//#define INRQ				(1U << 0)  // Initialization Request
#define SLAK				(1U << 1)  // Sleep Mode Request
#define INAK				(1U << 0)  // Initialization Acknowledge


/* Function prototypes */
void CAN_init(void);
void CAN_start(void);
void CAN_loopback(void);

void CAN_send(uint8_t *data);
int CAN_receive(uint8_t *data);


#endif /* CAN_L475_H_ */
