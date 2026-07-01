#ifndef __MOTOR_H__
#define __MOTOR_H__

#include  "tim.h"
#include "Com_debug.h"

/**
 * @brief 配置定时器，通道和速度值来控制电机的速度。
 * 
 * @note 速度值的范围为0-1000，超过1000会被视为无效输入。
 * 
 */
typedef struct
{
    TIM_HandleTypeDef *tim;
    uint16_t channel;
    uint16_t speed;
}Motor_Struct;


/**
 *
 *  @brief 传入的参数是比较值，最大为1000，默认值为200。 
 * 
 * 
 * @param speed 速度值，范围为0-1000，默认值为200。
 * @return 无返回值。
 */
void Motor_SetSpeed(Motor_Struct *motor);


/**
 * @brief 启动电机，传入具体的电机结构体指针。
 * 
 * @param motor 电机结构体指针。
 * @return 无返回值。
 */
void Motor_Start(Motor_Struct *motor);


#endif
