#include "APP_Flight.h"

// 全局结构体变量
IMU_Data imu_data = {0};
Euler_struct euler_angle = {0};
Gyro_Struct last_gyro = {0};
float gyro_z_sum = 0; // 角速度积分得到的偏航角

/**
 * @brief 利用低通滤波器对陀螺仪数据进行滤波，并利用卡尔曼滤波器对加速度计数据进行滤波
 *
 */
void APP_Flight_Get_Euler_Angle(void)
{
    MPU6050_Get_IMU(&imu_data);
    // 角度数据抖动相对较小，一阶低通滤波器即可满足需求，计算速度快，简便
    imu_data.gyro.gyro_x = Common_Filter_LowPass(imu_data.gyro.gyro_x, last_gyro.gyro_x);
    imu_data.gyro.gyro_y = Common_Filter_LowPass(imu_data.gyro.gyro_y, last_gyro.gyro_y);
    imu_data.gyro.gyro_z = Common_Filter_LowPass(imu_data.gyro.gyro_z, last_gyro.gyro_z);

    last_gyro.gyro_x = imu_data.gyro.gyro_x;
    last_gyro.gyro_y = imu_data.gyro.gyro_y;
    last_gyro.gyro_z = imu_data.gyro.gyro_z;
    // debug_printf(":%d,%d,%d\n", imu_data.gyro.gyro_x, imu_data.gyro.gyro_y, imu_data.gyro.gyro_z);
    // 由于加速度数据抖动较大，使用卡尔曼滤波器对加速度数据进行滤波，滤波效果更好
    // 卡尔曼滤波公式：计算精度高，但计算量大，滤波效果好
    imu_data.accel.accel_x = Common_Filter_KalmanFilter(&kfs[0], imu_data.accel.accel_x);
    imu_data.accel.accel_y = Common_Filter_KalmanFilter(&kfs[1], imu_data.accel.accel_y);
    imu_data.accel.accel_z = Common_Filter_KalmanFilter(&kfs[2], imu_data.accel.accel_z);
    // debug_printf(":%d,%d,%d\n", imu_data.accel.accel_x, imu_data.accel.accel_y, imu_data.accel.accel_z);

 // 4. 通过加速度和角速度来计算当前飞机切斜的角度 => 姿态解算
    // 使用互补解算计算欧拉角 => 优先使用加速度解算 => 俯仰角和横滚角能够使用
    euler_angle.pitch = atan2(imu_data.accel.accel_x * 1.0, imu_data.accel.accel_z) / 3.14159 * 180;

    euler_angle.roll = atan2(imu_data.accel.accel_y * 1.0, imu_data.accel.accel_z) / 3.14159 * 180;

    // 偏航角 => 只能使用角速度积分
    // 16位ADC的值转换为°/s  => 量程是±2000°/s
    gyro_z_sum += (imu_data.gyro.gyro_z * 2000.0 / 32768.0) * 0.006;
    euler_angle.yaw = gyro_z_sum;
    // debug_printf(":%.2f,%0.2f,%0.2f\n", euler_angle.pitch, euler_angle.roll, euler_angle.yaw);
}
