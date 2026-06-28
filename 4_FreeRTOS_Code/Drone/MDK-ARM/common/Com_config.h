#ifndef __COM_CONFIG_H
#define __COM_CONFIG_H

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
}Remote_State;


/**
 * @brief 飞行状态枚举
 * 
 * @note 该枚举用于表示飞行器的飞行状态，IDLE表示空闲状态，NORMAL表示正常飞行状态，FIX_HEIGHT表示定高飞行状态，FAIL表示故障状态
 */
typedef enum
{
    IDLE = 0,
    NORMAL,
    FIX_HEIGHT,
    FAIL,
}Flight_State;


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

#endif
