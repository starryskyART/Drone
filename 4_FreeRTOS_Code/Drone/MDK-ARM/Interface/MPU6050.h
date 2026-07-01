#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "i2c.h"

// MPU6050 I2C address
#define MPU6050_ADDR 0x68
// MPU6050 register addresses
#define MPU6050_ADDR_WRITE 0xD0
#define MPU6050_ADDR_READ 0xD1
#define PWR_MGMT_1 0x6B // Power management register 1
#define GYROSCOPE_CONFIGURATION 0x1B // Gyroscope configuration register
#define Accelerometer_Configuration 0x1C // Accelerometer configuration register
#define Interrupt_Enable 0x38 // Interrupt enable register
#define USER_CONTROL 0x6A // User control register
#define SAMPLE_RATE_DIVIDER 0x19 // Sample rate divider register


void MPU6050_Init(void);
void MPU6050_ReadData(uint8_t reg, uint8_t *data);
void MPU6050_WriteData(uint8_t reg, uint8_t data);

#endif
