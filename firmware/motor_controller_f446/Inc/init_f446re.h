/*
 * init_f446re.h
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

#ifndef INIT_F446RE_H_
#define INIT_F446RE_H_

/* INCLUDES */
#include "stm32f4xx.h"

/* DEFINITIONS */
#define GPIOAEN 					(1U<<0)
#define CAN1EN						(1U << 25) // CAN1 clock enable

/* FUNCTION PROTOTYPES */
void SystemInit(void);

#endif /* INIT_F446RE_H_ */
