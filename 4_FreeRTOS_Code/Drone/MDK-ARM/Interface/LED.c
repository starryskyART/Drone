#include "LED.h"



/**
 * @brief 打开LED灯
 * 
 * 
 * @param LED
 */
void LED_Turn_On(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->Port, led->Pin, GPIO_PIN_RESET);
}


/**
 * @brief 关闭LED灯
 * 
 * 
 * @param LED
 */
void LED_Turn_Off(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->Port, led->Pin, GPIO_PIN_SET);
}

/**
 * @brief 翻转LED灯
 * 
 * 
 * @param LED
 */
void LED_Toggle(LED_Struct *led)
{
    HAL_GPIO_TogglePin(led->Port, led->Pin);
}

