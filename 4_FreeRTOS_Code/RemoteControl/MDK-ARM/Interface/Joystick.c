#include "Joystick.h"

uint16_t ADC_ConvertedValue[4] = {0}; // ADC转换后的值

/**
 * @brief 遥感初始化
 *
 */
void Joystick_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_ConvertedValue, 4);
}


/**
 * @brief 获取遥感值
 * @param joystick 遥感结构体指针
 */
void Joystick_GetValue(Joystick_Stuct *joystick)
{
    //不能随机的顺序读取ADC值，必须按照ADC通道的顺序读取，否则会出现数据错乱
    joystick->throttle = ADC_ConvertedValue[0]; // 油门
    joystick->yaw = ADC_ConvertedValue[1]; // 偏航角
    joystick->pitch = ADC_ConvertedValue[2]; // 俯仰角
    joystick->roll = ADC_ConvertedValue[3]; // 横滚角
}
