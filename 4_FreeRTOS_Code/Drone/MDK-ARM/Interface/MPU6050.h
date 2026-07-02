#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "i2c.h"
#include "Com_config.h"



// MPU6050 I2C address
#define MPU6050_ADDR 0x68
// MPU6050 register addresses
#define MPU6050_ADDR_WRITE 0xD0
#define MPU6050_ADDR_READ 0xD1
#define PWR_MGMT_1 0x6B                  // Power management register 1
#define PWR_MGMT_2 0x6C                  // Power management register 2
#define GYROSCOPE_CONFIGURATION 0x1B     // Gyroscope configuration register
#define Accelerometer_Configuration 0x1C // Accelerometer configuration register
#define Interrupt_Enable 0x38            // Interrupt enable register
#define USER_CONTROL 0x6A                // User control register
#define SAMPLE_RATE_DIVIDER 0x19         // Sample rate divider register
#define CONFIGURATION 0x1A               // Configuration register
// MPU6050 data registers
#define GYRO_XOUT_H 0x43                 // Gyroscope X-axis high byte
#define GYRO_XOUT_L 0x44                 // Gyroscope X-axis low byte
#define GYRO_YOUT_H 0x45                 // Gyroscope Y-axis high byte
#define GYRO_YOUT_L 0x46                 // Gyroscope Y-axis low byte
#define GYRO_ZOUT_H 0x47                 // Gyroscope Z-axis high byte
#define GYRO_ZOUT_L 0x48                 // Gyroscope Z-axis low byte
#define ACCEL_XOUT_H 0x3B                // Accelerometer X-axis high byte
#define ACCEL_XOUT_L 0x3C                // Accelerometer X-axis low byte
#define ACCEL_YOUT_H 0x3D                // Accelerometer Y-axis high byte
#define ACCEL_YOUT_L 0x3E                // Accelerometer Y-axis low byte
#define ACCEL_ZOUT_H 0x3F                // Accelerometer Z-axis high byte
#define ACCEL_ZOUT_L 0x40                // Accelerometer Z-axis low byte

void MPU6050_Init(void);
void MPU6050_ReadData(uint8_t reg, uint8_t *data);
void MPU6050_WriteData(uint8_t reg, uint8_t data);
void MPU6050_Get_Gyro(Gyro_Struct *gyro_data);
void MPU6050_Get_Accel(Accel_Struct *accel_data);
void MPU6050_Get_IMU(IMU_Data *imu_data);

#endif
