/*
 * main.h
 * For L475
 *  Created on: May 28, 2026
 *      Author: joshu
 */

#ifndef MAIN_H_
#define MAIN_H_


/* Include necessary headers */
#include <stdio.h>

#include "uart_enable_l475.h"
#include "CAN_l475.h"
#include "init_l475.h"


/* Definitions */
#define DEBUG_PRINT(...) printf(__VA_ARGS__)


/* Function Prototypes */
void delay(volatile int count);


#endif /* MAIN_H_ */
