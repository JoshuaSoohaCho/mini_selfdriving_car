/*
 * i2c_l475.c
 *
 *  Created on: Jun 7, 2026
 *      Author: joshu
 */

#include "i2c_l475.h"
// program PC0 (i2c3_scl) and PC1 (i2c3_sda)

void i2c_init(void) {
	// Set PC0 and PC1 to pull up
	GPIOC->PUPDR |= (1 << 0);
	GPIOC->PUPDR &= ~(1 << 1);

	GPIOC->PUPDR |= (1 << 2);
	GPIOC->PUPDR &= ~(1 << 3);

	// Set PC0 and PC1 to alternate function mode
	GPIOC->MODER &= ~(1 << 0);
	GPIOC->MODER |= (1 << 1);

	GPIOC->MODER &= ~(1 << 2);
	GPIOC->MODER |= (1 << 3);

	// Set PC0 and PC1 to AF4
	GPIOC->AFR[0] &= ~(1 << 0);
	GPIOC->AFR[0] &= ~(1 << 1);
	GPIOC->AFR[0] |= (1 << 2);
	GPIOC->AFR[0] &= ~(1 << 3);

	GPIOC->AFR[0] &= ~(1 << 4);
	GPIOC->AFR[0] &= ~(1 << 5);
	GPIOC->AFR[0] |= (1 << 6);
	GPIOC->AFR[0] &= ~(1 << 7);

	// Set PC0 and PC1 to Open Drain
	GPIOC->OTYPER |= PC0_OPENDRAIN;
	GPIOC->OTYPER |= PC1_OPENDRAIN;

	// Disable I2C first to configure timing
	I2C3->CR1 &= ~PE;


//	I2C3->CR1 |= (NACKIE);

	I2C3->TIMINGR = 0x20303E5D; // 16Mhz (HSI), 100 kHz

	// Enable I2C
	I2C3->CR1 |= PE;
	printf("initialization complete.\n\r");
}

void i2c_write_reg(uint8_t dev_addr, uint8_t reg, uint8_t data){
	// wait for busy
	while(I2C3->ISR & BUSY){}

	I2C3->ICR |= STOPCF | NACKCF;

	I2C3->CR2 = (dev_addr << 1) | (2 << 16) | START;

	// Wait TXIS (ready to send)
	while(!(I2C3->ISR & TXIS)){}

	I2C3->TXDR = reg;

	while(!(I2C3->ISR &TXIS)){}

	I2C3->TXDR = data;

	while(!(I2C3->ISR & STOPF)){}

//	I2C3->CR2 |= STOP;
}

uint8_t i2c_read_reg(uint8_t dev_addr, uint8_t reg){
	printf("read_reg()\n\r");
	uint8_t data;


	while(I2C3->ISR & BUSY){}
	printf("NOT BUSY\n\r");

	I2C3->CR2 = (dev_addr << 1) | (1 << 16) | START;

	while(!(I2C3->ISR & TXIS)) {}
	printf("TXIS\n\r");

	I2C3->TXDR = reg;

	while(!(I2C3->ISR & TC)){}
	printf("TC\n\r");

	I2C3->CR2 = (dev_addr << 1) | (1 << 16) | START;
	I2C3->CR2 |= RD_WRN;

	while(!(I2C3->ISR & RXNE)){}
	printf("RXNE\n\r");
	data = I2C3->RXDR;

	while(!(I2C3->ISR & TC)) {}
	printf("TC_2\n\r");
	I2C3->CR2 |= STOP;

	return data;
}


void i2c_bus_scan(void)
{
    printf("Scanning I2C bus...\n\r");

    for (uint8_t addr = 1; addr < 128; addr++)
    {
        // Wait until bus is free
        int timeout = 100000;
        while ((I2C3->ISR & BUSY) && timeout--);

        if (timeout <= 0)
        {
            printf("BUS STUCK, resetting...\n\r");

            // Reset I2C
            I2C3->CR1 &= ~PE;
            I2C3->CR1 |= PE;

            continue;
        }

        // Clear flags
        I2C3->ICR |= STOPCF | NACKCF;

        // Setup transfer (WRITE, 1 byte but we won't send it)
        I2C3->CR2 =
            (addr << 1) |    // address
            (1 << 16) |      // NBYTES = 1
            START;

        // Wait for either TXIS or NACK
        timeout = 100000;
        while (!(I2C3->ISR & (TXIS | NACKF)) && timeout--);

        if (timeout <= 0)
        {
            printf("Timeout at 0x%X\n\r", addr);
            continue;
        }

        if (I2C3->ISR & NACKF)
        {
            // No device
            I2C3->ICR |= NACKCF;
        }
        else if (I2C3->ISR & TXIS)
        {
            // Device Acked
            printf("Found device at 0x%X\n\r", addr);
            I2C3->TXDR = 0x00;
        }

        // Generate STOP manually
        I2C3->CR2 |= STOP;

        // Wait for STOP
        timeout = 100000;
        while (!(I2C3->ISR & STOPF) && timeout--);

        I2C3->ICR |= STOPCF;
    }

    printf("Scan complete\n\r");
}
