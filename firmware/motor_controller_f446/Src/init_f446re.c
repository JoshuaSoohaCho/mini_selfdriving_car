/*
 * initialization_f446re.c
 *
 *  Created on: May 31, 2026
 *      Author: joshu
 */

// Initialize Clock access to GPIOA
void init_GPIOA_clock(void) {
	RCC->AHB1ENR |= GPIOAEN; // Enable clock for GPIOA
}
