#include "Power.h"





/**
 * @brief  初始化电源管理模块
 * @retval None
 */
void Power_Start(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // 设置PA15引脚为低电平，打开电源
    vTaskDelay(100); // 延时100ms，等待电源稳定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); // 设置PA15引脚为高电平，关闭电源
}


/**
 * @brief  关闭电源管理模块
 * 
 */
void Power_Shutdown(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // 设置PA15引脚为低电平，打开电源
    vTaskDelay(100); // 延时100ms，等待电源稳定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); // 设置PA15引脚为高电平，关闭电源
    vTaskDelay(200); // 延时200ms，等待电源稳定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // 设置PA15引脚为低电平，打开电源
    vTaskDelay(100); // 延时100ms，等待电源稳定
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); // 设置PA15引脚为高电平，关闭电源
}
