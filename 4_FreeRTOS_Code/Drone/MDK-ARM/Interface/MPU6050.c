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
    MPU6050_WriteData(SAMPLE_RATE_DIVIDER, 0x01); // Set the sample rate divider to 500Hz
}
