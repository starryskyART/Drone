#include "APP_Flight.h"

// 全局结构体变量
IMU_Data imu_data = {0};
Euler_struct euler_data = {0};
Gyro_Struct last_gyro = {0};



/**
 * @brief 利用低通滤波器对陀螺仪数据进行滤波，并利用卡尔曼滤波器对加速度计数据进行滤波
 *
 */
void APP_Flight_Get_Euler_Angle(void)
{
    MPU6050_Get_IMU(&imu_data);
    // 低通滤波公式
    imu_data.gyro.gyro_x = Common_Filter_LowPass(imu_data.gyro.gyro_x, last_gyro.gyro_x);
    imu_data.gyro.gyro_y = Common_Filter_LowPass(imu_data.gyro.gyro_y, last_gyro.gyro_y);
    imu_data.gyro.gyro_z = Common_Filter_LowPass(imu_data.gyro.gyro_z, last_gyro.gyro_z);

    last_gyro.gyro_x = imu_data.gyro.gyro_x;
    last_gyro.gyro_y = imu_data.gyro.gyro_y;
    last_gyro.gyro_z = imu_data.gyro.gyro_z;
    debug_printf(":%d,%d,%d\n", imu_data.gyro.gyro_x, imu_data.gyro.gyro_y, imu_data.gyro.gyro_z);
    
}
