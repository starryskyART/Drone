#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "adc.h"
#include "Com_tool.h"



/**
 * @brief Joystick structure definition
 * 
 */
typedef struct
{
    int16_t throttle; // 油门
    int16_t yaw; // 偏航角
    int16_t pitch; // 俯仰角
    int16_t roll; // 横滚角
}Joystick_Stuct;

void Joystick_Init(void);
void Joystick_GetValue(Joystick_Stuct *joystick);


#endif
