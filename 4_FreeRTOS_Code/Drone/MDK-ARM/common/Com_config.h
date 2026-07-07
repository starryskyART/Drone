#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "main.h"

/**
 * @brief 遥控器连接状态枚举
 *
 * @note 该枚举用于表示遥控器的连接状态，REMOTE_CONNECTED表示遥控器已连接，REMOTE_DISCONNECTED表示遥控器已断开连接
 */
typedef enum
{
    REMOTE_CONNECTED = 0,
    REMOTE_DISCONNECTED,
} Remote_State;

/**
 * @brief 飞行状态枚举
 *
 * @note 该枚举用于表示飞行器的飞行状态，IDLE表示空闲状态，NORMAL表示正常飞行状态，FIX_HIGHT表示定高飞行状态，FAIL表示故障状态
 */
typedef enum
{
    IDLE = 0,
    NORMAL,
    FIX_HIGHT,
    FAIL,
} Flight_State;

/**
 * @brief 遥控器数据结构体
 *
 */
typedef struct
{
    int16_t throttle; // 油门
    int16_t yaw;      // 偏航角
    int16_t pitch;    // 俯仰角
    int16_t roll;     // 横滚角
    uint8_t shutdown; // 遥感关机标志位，0不关机，1关机
    uint8_t fix_high; // 遥感定高
} Remote_Data;        // 遥感数据结构体

/**
 * @brief 遥控解锁状态机
 *
 */
typedef enum
{
    Free = 0,  // 遥控器未解锁
    MAX,       // 油门达到最大值持续时间
    LEAVE_MAX, // 油门达到最大值后离开最大值最长时间
    MIN,       // 油门最短持续时间
    UNLOCKED,  // 遥控器已解锁
} Throttle_State;

/**
 * @brief 陀螺仪角速度数据结构体
 * @note 16位ADC的值
 * 
 */
typedef struct
{
    int16_t gyro_x; // 陀螺仪X轴数据，向右飞为正
    int16_t gyro_y; // 陀螺仪Y轴数据，向前飞为正
    int16_t gyro_z; // 陀螺仪Z轴数据，向左转向为正
} Gyro_Struct;

/**
 * @brief 加速度计数据结构体
 * @note 16位ADC的值
 *
 */
typedef struct
{
    int16_t accel_x; // 加速度计X轴数据，向前加速度为正
    int16_t accel_y; // 加速度计Y轴数据，向右加速度为正
    int16_t accel_z; // 加速度计Z轴数据,朝上加速度为正
} Accel_Struct;

/**
 * @brief 角速度和加速度数据结构体
 * @note 16位ADC的值
 *
 */
typedef struct
{
    Gyro_Struct gyro;   // 陀螺仪数据
    Accel_Struct accel; // 加速度计数据
} IMU_Data;

/**
 * @brief 解算得到的欧拉角
 *
 */
typedef struct
{
    float yaw;   // 偏航角
    float pitch; // 俯仰角
    float roll;  // 横滚角
} Euler_struct;

#endif
