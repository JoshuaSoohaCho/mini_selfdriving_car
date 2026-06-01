/*
 * CAN_f446re.h
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

#ifndef CAN_F446RE_H_
#define CAN_F446RE_H_

/* INCLUDES */
#include "stm32f4xx.h"

/* DEFINITIONS */
#define RCC_APB1ENR_CAN1EN			(1U << 25) // CAN1 clock enable
#define CAN_INRQ					(1U << 0)  // Initialization Request

/* FUNCTION PROTOTYPES */
void CAN_init(void);

#endif /* CAN_F446RE_H_ */
