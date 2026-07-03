#include "APP_Flight.h"

Gyro_Struct gyro_data = {0};   // 陀螺仪数据结构体
Accel_Struct accel_data = {0}; // 加速度计数据结构体

/**
 * @brief 计算欧拉角
 *
 */
void APP_Flight_Get_Euler_Angle(void)
{
    // MPU6050_Get_Gyro(&gyro_data);
    MPU6050_Get_Accel(&accel_data);

    // debug_printf(":%d %d %d\n", gyro_data.gyro_x, gyro_data.gyro_y, gyro_data.gyro_z);
    // debug_printf(":%d,%d,%d\n", accel_data.accel_x, accel_data.accel_y, accel_data.accel_z);
}
