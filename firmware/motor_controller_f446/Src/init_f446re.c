/*
 * initialization_f446re.c
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */
#include "init_f446re.h"

// Initialize Clock access to GPIOA
void SystemInit(void){
	// Enable clock for GPIOA	RCC->AHB1ENR |= GPIOAEN;

	// Additional system initialization code can be added here if needed}
