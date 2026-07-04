#include "MPU6050.h"

/// Global variables to store the gyroscope offsets and accelerometer offsets
int32_t gyro_offset_x = 0;
int32_t gyro_offset_y = 0;
int32_t gyro_offset_z = 0;

int32_t accel_offset_x = 0;
int32_t accel_offset_y = 0;
int32_t accel_offset_z = 0;

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

    // Calculate the offset of the MPU6050 sensor
    MPU6050_Calculate_Offset();
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
    gyro_data->gyro_x = ((high_byte << 8) | low_byte) - gyro_offset_x;

    MPU6050_ReadData(GYRO_YOUT_H, &high_byte);
    MPU6050_ReadData(GYRO_YOUT_L, &low_byte);
    gyro_data->gyro_y = ((high_byte << 8) | low_byte) - gyro_offset_y;

    MPU6050_ReadData(GYRO_ZOUT_H, &high_byte);
    MPU6050_ReadData(GYRO_ZOUT_L, &low_byte);
    gyro_data->gyro_z = ((high_byte << 8) | low_byte) - gyro_offset_z;
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
    accel_data->accel_x = ((high_byte << 8) | low_byte) - accel_offset_x;
    MPU6050_ReadData(ACCEL_YOUT_H, &high_byte);
    MPU6050_ReadData(ACCEL_YOUT_L, &low_byte);
    accel_data->accel_y = ((high_byte << 8) | low_byte) - accel_offset_y;
    MPU6050_ReadData(ACCEL_ZOUT_H, &high_byte);
    MPU6050_ReadData(ACCEL_ZOUT_L, &low_byte);
    accel_data->accel_z = ((high_byte << 8) | low_byte) - accel_offset_z;
}

/**
 * @brief Calculates the offset of the MPU6050 sensor.
 *
 */
void MPU6050_Calculate_Offset(void)
{
    Accel_Struct accel_current = {0};
    Accel_Struct accel_last = {0};
    MPU6050_Get_Accel(&accel_last);
    uint8_t count = 0;
    // Zero the offset values
    while (count < 100)
    {
        MPU6050_Get_Accel(&accel_current);
        if (abs(accel_current.accel_x - accel_last.accel_x) < 400 && abs(accel_current.accel_y - accel_last.accel_y) < 400 && abs(accel_current.accel_z - accel_last.accel_z) < 400)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        accel_last = accel_current;
        vTaskDelay(6);
    }
    // Drone is stationary, calculate the offset
    IMU_Data imu_data = {0};
    int32_t gyro_sum_x = 0;
    int32_t gyro_sum_y = 0;
    int32_t gyro_sum_z = 0;

    int32_t accel_sum_x = 0;
    int32_t accel_sum_y = 0;
    int32_t accel_sum_z = 0;
    for (uint8_t i = 0; i < 100; i++)
    {
        MPU6050_Get_IMU(&imu_data);
        gyro_sum_x += (imu_data.gyro.gyro_x) - 0;
        gyro_sum_y += (imu_data.gyro.gyro_y) - 0;
        gyro_sum_z += (imu_data.gyro.gyro_z) - 0;
        // Note: The Z-axis gyroscope offset is adjusted by subtracting 16384 to account for the effect of gravity on the Z-axis.
        // This is because when the drone is stationary, the Z-axis accelerometer will measure the acceleration due to gravity, which is approximately 1g (16384 in raw data). Therefore, we need to subtract this value from the Z-axis gyroscope reading to get the true offset.
        accel_sum_x += (imu_data.accel.accel_x) - 0;
        accel_sum_y += (imu_data.accel.accel_y) - 0;
        accel_sum_z += (imu_data.accel.accel_z) - 16384;

        vTaskDelay(6);
    }
    accel_offset_x = accel_sum_x / 100;
    accel_offset_y = accel_sum_y / 100;
    accel_offset_z = accel_sum_z / 100;

    gyro_offset_x = gyro_sum_x / 100;
    gyro_offset_y = gyro_sum_y / 100;
    gyro_offset_z = gyro_sum_z / 100;
}

void MPU6050_Get_IMU(IMU_Data *imu_data)
{
    MPU6050_Get_Gyro(&imu_data->gyro);
    MPU6050_Get_Accel(&imu_data->accel);
}
