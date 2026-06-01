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

/* FUNCTION PROTOTYPES */
void init_GPIOA_clock(void);

#endif /* INIT_F446RE_H_ */
