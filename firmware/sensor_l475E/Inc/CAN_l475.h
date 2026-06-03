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


/* Definitions */
#define RCC_APB1ENR1_CAN1EN			(1U << 0)  // CAN1 clock enable
#define RCC_AHB2ENR_GPIOAEN			(1U << 0)  // GPIOA clock enable

#define CAN_INRQ					(1U << 0)  // Initialization Request
#define CAN_FMR_FINIT				(1U << 0)  // Filter Initialization Mode
#define CAM1_FA1R_FACT0				(1U << 0)  // Filter 0 activation bit


/* Function prototypes */
void CAN_init(void);
void CAN_send(uint8_t data);


#endif /* CAN_L475_H_ */
