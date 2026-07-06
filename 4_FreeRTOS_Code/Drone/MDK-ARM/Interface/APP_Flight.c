#include "APP_Flight.h"

// 全局结构体变量
IMU_Data imu_data = {0};        // 陀螺仪和加速度计数据
Euler_struct euler_angle = {0}; // 欧拉角
Gyro_Struct last_gyro = {0};    // 上一次陀螺仪数据
float gyro_z_sum = 0;           // 角速度积分得到的偏航角
extern Remote_Data remote_data; // 遥控器数据结构体变量
// PID控制结构体变量
PID_TypeDef pitch_pid = {.Kp = 0, .Ki = 0, .Kd = 0};  // 俯仰角PID控制结构体（外环）
PID_TypeDef gyro_y_pid = {.Kp = 0, .Ki = 0, .Kd = 0}; // 俯仰角角速度PID控制结构体（内环）

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
    /*========================================两种姿态解算方法========================================*/
    // 1. 通过加速度和角速度来计算当前飞机切斜的角度 => 姿态解算
    // 使用互补解算计算欧拉角 => 优先使用加速度解算 => 俯仰角和横滚角能够使用
    // euler_angle.pitch = atan2(imu_data.accel.accel_x * 1.0, imu_data.accel.accel_z) / 3.14159 * 180;

    // euler_angle.roll = atan2(imu_data.accel.accel_y * 1.0, imu_data.accel.accel_z) / 3.14159 * 180;

    // 偏航角 => 只能使用角速度积分
    // 16位ADC的值转换为°/s  => 量程是±2000°/s
    // gyro_z_sum += (imu_data.gyro.gyro_z * 2000.0 / 32768.0) * 0.006;
    // euler_angle.yaw = gyro_z_sum;
    // debug_printf(":%.2f,%0.2f,%0.2f\n", euler_angle.pitch, euler_angle.roll, euler_angle.yaw);

    // 2.通过四元数解算欧拉角
    Common_IMU_GetEulerAngle(&imu_data, &euler_angle, 0.006);
    debug_printf(":%d,%d,%d\n", (int)euler_angle.pitch, (int)euler_angle.roll, (int)euler_angle.yaw);
    /*========================================两种姿态解算方法========================================*/
}

/**
 * @brief PID控制计算函数
 *
 *
 */
void APP_Flight_PID_Process(void)
{
    // 外环PID计算 => 计算俯仰角的期望角速度
    // 遥控器的俯仰角范围是0 - 1000，控制期望角度范围是-10° - 10°
    pitch_pid.desire = (remote_data.pitch); // 期望值为遥控器的俯仰角
    pitch_pid.measure = euler_angle.pitch;  // 测量值为当前俯仰角
    gyro_y_pid.measure = imu_data.gyro.gyro_x; // 内环测量值为当前角速度

    Com_PID_Calculate_Chain(&pitch_pid, &gyro_y_pid);

    // debug_printf(":%d,%d,%d\n", (int)pitch_pid.err, (int)pitch_pid.output, (int)gyro_y_pid.output);


}
