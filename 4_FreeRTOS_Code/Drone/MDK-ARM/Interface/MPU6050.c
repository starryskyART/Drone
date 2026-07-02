#include "MPU6050.h"

/**
 * @brief Writes data to a specific register of the MPU6050 sensor.
 *
 * @param reg The register address to write to.
 *
 * @param data The data to write to the register.
 *
 */
void MPU6050_WriteData(uint8_t reg, uint8_t data)
{
    // Use HAL_I2C_Mem_Write to write data to the MPU6050 register
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

/**
 * @brief Reads data from a specific register of the MPU6050 sensor.
 *
 * @param reg the register address to read from.
 *
 * @param data pointer to the variable where the read data will be stored.
 *
 */
void MPU6050_ReadData(uint8_t reg, uint8_t *data)
{
    // Use HAL_I2C_Mem_Read to read data from the MPU6050 register
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_READ, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 1000);
}

/**
 * @brief Initializes the MPU6050 sensor.
 *
 */
void MPU6050_Init(void)
{
    MPU6050_WriteData(PWR_MGMT_1, 0x80); // Reset the MPU6050, all registers are set to their default values
    uint8_t data = 0;
    while (data != 0x40)
    {
        MPU6050_ReadData(PWR_MGMT_1, &data); // Wait for the reset to complete
    }
    MPU6050_WriteData(PWR_MGMT_1, 0x00); // Wake up the MPU6050, set the sleep bit to 0

    // Set the gyroscope configuration to ±2000 degrees per second
    MPU6050_WriteData(GYROSCOPE_CONFIGURATION, 0x18); // Set the gyroscope configuration to ±2000 degrees per second
    // Set the accelerometer configuration to ±2g
    MPU6050_WriteData(Accelerometer_Configuration, 0x00); // Set the accelerometer configuration to ±2g
    // Disable interrupts
    MPU6050_WriteData(Interrupt_Enable, 0x00); // Disable interrupts
    // User control register configuration
    MPU6050_WriteData(USER_CONTROL, 0x00); // Disable FIFO and I2C Master mode
    // Set the sample rate divider to 500Hz
    // according to the Nyquist theorem, the sampling frequency should be greater than twice the signal frequency, so the sampling frequency should be greater than 500Hz
    // Since the task scheduling frequency is 6ms once, which is equivalent to 166.67Hz, the sampling frequency should be greater than 333.33Hz, so it is set to 500Hz
    MPU6050_WriteData(SAMPLE_RATE_DIVIDER, 0x01);
    // Set the configuration register to 1, which sets the digital low-pass filter to 188Hz
    MPU6050_WriteData(CONFIGURATION, 1);
    // set the system clock source to PLL with X axis gyroscope reference
    MPU6050_WriteData(PWR_MGMT_1, 0x01);
    // Enable the PWR_MGMT_2 register to enable the accelerometer and gyroscope
    MPU6050_WriteData(PWR_MGMT_2, 0x00);
}

/**
 * @brief Gets the gyroscope data from the MPU6050 sensor.
 *
 * @param gyro_data pointer to the Gyro_Struct where the gyroscope data will be stored.
 *
 *
 */
void MPU6050_Get_Gyro(Gyro_Struct *gyro_data)
{
    uint8_t high_byte, low_byte = 0;
    // storeg the values of the gyroscope registers from 0x43 to 0x48, high byte first, low byte second, in XYZ order
    MPU6050_ReadData(GYRO_XOUT_H, &high_byte);
    MPU6050_ReadData(GYRO_XOUT_L, &low_byte);
    gyro_data->gyro_x = ((high_byte << 8) | low_byte);

    MPU6050_ReadData(GYRO_YOUT_H, &high_byte);
    MPU6050_ReadData(GYRO_YOUT_L, &low_byte);
    gyro_data->gyro_y = ((high_byte << 8) | low_byte);

    MPU6050_ReadData(GYRO_ZOUT_H, &high_byte);
    MPU6050_ReadData(GYRO_ZOUT_L, &low_byte);
    gyro_data->gyro_z = ((high_byte << 8) | low_byte);
}

/**
 * @brief Gets the accelerometer data from the MPU6050 sensor.
 *
 * @param accel_data pointer to the Accel_Struct where the accelerometer data will be stored.
 *
 */
void MPU6050_Get_Accel(Accel_Struct *accel_data)
{
    uint8_t high_byte, low_byte = 0;
    // storeg the values of the accelerometer registers from 0x3B to 0x40, high byte first, low byte second, in XYZ order
    MPU6050_ReadData(ACCEL_XOUT_H, &high_byte);
    MPU6050_ReadData(ACCEL_XOUT_L, &low_byte);
    accel_data->accel_x = ((high_byte << 8) | low_byte);
    MPU6050_ReadData(ACCEL_YOUT_H, &high_byte);
    MPU6050_ReadData(ACCEL_YOUT_L, &low_byte);
    accel_data->accel_y = ((high_byte << 8) | low_byte);
    MPU6050_ReadData(ACCEL_ZOUT_H, &high_byte);
    MPU6050_ReadData(ACCEL_ZOUT_L, &low_byte);
    accel_data->accel_z = ((high_byte << 8) | low_byte);
}

void MPU6050_Get_IMU(IMU_Data *imu_data)
{
    MPU6050_Get_Gyro(&imu_data->gyro);
    MPU6050_Get_Accel(&imu_data->accel);
}
