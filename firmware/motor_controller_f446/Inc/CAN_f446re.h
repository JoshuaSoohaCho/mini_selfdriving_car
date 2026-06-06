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



/* DEFINITIONS */
#define CAN1EN			(1U << 25) // CAN1 clock enable



#define CAN_INRQ					(1U << 0)  // Initialization Request
#define FINIT				(1U << 0)  // Filter Initialization Mode
#define CAM1_FA1R_FACT0				(1U << 0)  // Filter 0 activation bit
#define SLEEP				(1U << 1)  // Sleep Mode
#define SLAK				(1U << 1)  // Sleep Mode Request
#define INAK				(1U << 0)  // Initialization Acknowledge




/* FUNCTION PROTOTYPES */
void CAN_init(void);
uint8_t CAN_receive(void);


#endif /* CAN_F446RE_H_ */
