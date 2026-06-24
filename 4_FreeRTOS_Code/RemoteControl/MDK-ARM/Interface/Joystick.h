#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "adc.h"


/**
 * @brief Joystick structure definition
 * 
 */
typedef struct
{
    uint16_t throttle; // 油门
    uint16_t yaw; // 偏航角
    uint16_t pitch; // 俯仰角
    uint16_t roll; // 横滚角
}Joystick_Stuct;

void Joystick_Init(void);
void Joystick_GetValue(Joystick_Stuct *joystick);


#endif
