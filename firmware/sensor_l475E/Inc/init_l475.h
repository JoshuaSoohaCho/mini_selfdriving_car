/*
 * init_l475.h
 *
 *  Created on: Jun 4, 2026
 *      Author: joshu
 */

#ifndef INIT_L475_H_
#define INIT_L475_H_

/* Includes */
#include "stm32l475xx.h"

/* Definitions */
#define GPIOBEN 					(1U << 1) 	// GPIOB clock enable
#define GPIOCEN						(1U << 2)	// GPIOC clock enable
#define CAN1EN						(1U << 25)  // CAN1 clock enable
#define I2C3EN						(1U << 23) 	// I2C3 clock enable

#define HSION 						(1U << 8)  	// HSI clock enable
#define HSIRDY 						(1U << 10) 	// HSI clock ready flag
#define HSI16_SEL 					(1U << 0)  	// HSI16 selected as system clock

/* Function Prototypes */
void System_Init(void);

#endif /* INIT_L475_H_ */
