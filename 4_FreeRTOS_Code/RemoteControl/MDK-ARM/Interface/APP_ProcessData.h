#ifndef __APP_PROCESSDATA_H
#define __APP_PROCESSDATA_H


#include "Joystick.h"
#include "KEY.h"
#include "Com_debug.h"


typedef struct
{
    int16_t throttle; // 油门
    int16_t yaw;      // 偏航角
    int16_t pitch;    // 俯仰角
    int16_t roll;     // 横滚角
    uint8_t shutdown;  // 遥感关机标志位，0不关机，1关机
    uint8_t fix_high;  // 遥感定高
} Remote_Data; // 遥感数据结构体

void APP_ProcessKeyData(void);
void APP_ProcessJoystickData(void);
void APP_CalibrateJoystickData(void);


#endif
