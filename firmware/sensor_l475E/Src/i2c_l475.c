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

void i2c_write_reg(uint8_t dev_addr, uint8_t reg, uint8_t data)
{
    while (I2C3->ISR & BUSY);

    I2C3->ICR |= STOPCF | NACKCF;

    I2C3->CR2 =
        (dev_addr << 1) |
        (2 << 16) |     // reg + data
        START |
        AUTOEND;

    int timeout = 100000;

    while (!(I2C3->ISR & TXIS) && !(I2C3->ISR & NACKF) && timeout--);

    if (I2C3->ISR & NACKF)
    {
        printf("NACK!\n\r");
        I2C3->ICR |= NACKCF;
        return;
    }

    I2C3->TXDR = reg;

    while (!(I2C3->ISR & TXIS));
    I2C3->TXDR = data;

    while (!(I2C3->ISR & STOPF));
    I2C3->ICR |= STOPCF;
}

void i2c_reset(void){
	I2C3->CR1 &= ~PE;
	delay(1000);
	I2C3->CR1 |= PE;
	I2C3->ICR |= STOPCF | NACKCF;
}


void i2c_write(uint8_t dev_addr, uint8_t data)
{
    int timeout = 100000;
    while ((I2C3->ISR & BUSY) && timeout--);

    if (timeout <= 0)
    {
        printf("BUSY stuck\n\r");
        I2C3->CR1 &= ~PE;
        I2C3->CR1 |= PE;
        return;
    }

    I2C3->ICR |= STOPCF | NACKCF;

    I2C3->CR2 =
        (dev_addr << 1) |
        (1 << 16) |
        START |
        AUTOEND;

    while (!(I2C3->ISR & TXIS));
    I2C3->TXDR = data;

    while (!(I2C3->ISR & STOPF));
    I2C3->ICR |= STOPCF;
}

uint8_t i2c_read_reg(uint8_t dev_addr, uint8_t reg)
{
    uint8_t data;

    // Wait until bus free
    while (I2C3->ISR & BUSY);

    // -------- STEP 1: WRITE REGISTER ADDRESS --------
    I2C3->CR2 =
        (dev_addr << 1) |
        (1 << 16) |   // 1 byte
        START;

    while (!(I2C3->ISR & TXIS));

    I2C3->TXDR = reg;

    while (!(I2C3->ISR & TC));

    // -------- STEP 2: REPEATED START READ --------
    I2C3->CR2 =
        (dev_addr << 1) |
        (1 << 16) |   // 1 byte
        RD_WRN |      // ✅ NOW correct
        START |
        AUTOEND;

    while (!(I2C3->ISR & RXNE));

    data = I2C3->RXDR;

    while (!(I2C3->ISR & STOPF));

    I2C3->ICR |= STOPCF | NACKCF;

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

uint16_t bh1750_read(void)
{
    uint8_t high, low;

    while (I2C3->ISR & BUSY);

    I2C3->CR2 =
        (0x23 << 1) |
        (2 << 16) |   // 🔥 2 bytes
        START |
        RD_WRN |      // READ
        AUTOEND;

    // Wait for first byte
    while (!(I2C3->ISR & RXNE));
    high = I2C3->RXDR;

    // Wait for second byte
    while (!(I2C3->ISR & RXNE));
    low = I2C3->RXDR;

    // Combine
    uint16_t value = (high << 8) | low;

    return value;
}

void VL53L0X_Init(void)
{
    // Basic reset sequence
    i2c_write_reg(VL53_ADDR, 0x88, 0x00);

    i2c_write_reg(VL53_ADDR, 0x80, 0x01);
    i2c_write_reg(VL53_ADDR, 0xFF, 0x01);
    i2c_write_reg(VL53_ADDR, 0x00, 0x00);


    i2c_write_reg(VL53_ADDR, 0x00, 0x01);
    i2c_write_reg(VL53_ADDR, 0xFF, 0x00);
    i2c_write_reg(VL53_ADDR, 0x80, 0x00);

    i2c_write_reg(VL53_ADDR, 0x00, 0x02);

    printf("VL530X init complete...\n\r");
    delay(100000); // small delay
}

uint16_t VL53L0X_ReadDistance(void)
{
    uint8_t high = i2c_read_reg(VL53_ADDR, 0x1E);
    uint8_t low  = i2c_read_reg(VL53_ADDR, 0x1F);

    uint16_t distance = (high << 8) | low;

    return distance;
}

