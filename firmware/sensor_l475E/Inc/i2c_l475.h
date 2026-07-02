/*
 * i2c_l475.h
 *
 *  Created on: Jun 7, 2026
 *      Author: joshu
 */

#ifndef I2C_L475_H_
#define I2C_L475_H_

#include "stm32l475xx.h"
#include "stdio.h"
#include "main.h"

// OTYPER Bits
#define PC0_OPENDRAIN 			(1 << 0)
#define PC1_OPENDRAIN			(1 << 1)

// CR1 Bits
#define PE						(1 << 0)
#define NACKIE					(1 << 4)

// ISR Bits
#define TXIS					(1 << 1)
#define RXNE					(1 << 2)
#define NACKF					(1 << 4)
#define STOPF					(1 << 5)
#define TC						(1 << 6)
#define BUSY					(1 << 15)

// CR2 Bits
#define RD_WRN					(1 << 10)
#define START					(1 << 13)
#define STOP					(1 << 14)
#define AUTOEND					(1 << 25)

// ICR Bits
#define NACKCF					(1 << 4)
#define STOPCF					(1 << 5)

// I2C Addresses
#define VL53_ADDR				0x29


void i2c_init(void);
void i2c_reset(void);
void i2c_write_reg(uint8_t dev_addr, uint8_t reg, uint8_t data);
void i2c_write(uint8_t dev_addr, uint8_t data);
uint8_t i2c_read_reg(uint8_t dev_addr, uint8_t reg);
void i2c_bus_scan(void);
uint16_t bh1750_read(void);
void VL53L0X_Init(void);
uint16_t VL53L0X_ReadDistance(void);

#endif /* I2C_L475_H_ */
