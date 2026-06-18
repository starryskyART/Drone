#include "Motor.h"



void Motor_SetSpeed(Motor_Struct *motor)
{
    if (motor->speed > 1000)
    {
        //电机转速超过1000自动视为无效输入，打印调试信息并返回
        debug_printf("Motor speed out of range: %d\n", motor->speed);
        return;
    }
    //设置电机转速
    __HAL_TIM_SET_COMPARE(motor->tim, motor->channel, motor->speed);
}

void Motor_Start(Motor_Struct *motor)
{
    //启动电源
    HAL_TIM_PWM_Start(motor->tim, motor->channel);
}
